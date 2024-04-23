// Copyright Astralwater Interactive. All Rights Reserved.
//Astral Water Includes
#include "Actors/Pawns/Divebot.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "DiverController.h"
#include "Actors/PhotographyCamera.h"
#include "OnlineGameMode.h"
#include "Components/DivebotMovementComponent.h"
#include "HotbarWidget.h"
#include "Widgets/HUDWidget.h"


//Unreal Engine Includes
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "DivebotCameraOverlayWidget.h"
#include "ProjectAbyssSaveGame.h"


// Sets default values
ADivebot::ADivebot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UDivebotMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComponent"));
	FlashlightComponent->SetupAttachment(FirstPersonCameraComponent);
	FlashlightComponent->Intensity = 7000.0f;
	FlashlightComponent->OuterConeAngle = 30.0f;

	// Create Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Divebot Mesh"));
	MeshComponent->SetupAttachment(GetCapsuleComponent());
	MeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = Cast<UDivebotMovementComponent>(GetMovementComponent());

	//Disable crouching
	MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = false;
}

ADivebot::~ADivebot()
{
}

void ADivebot::BeginPlay()
{
	Super::BeginPlay();

	BateryLifeOG = BatteryLifeInSeconds;

	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{
		MyGameState->SetBot(this);
		player = MyGameState->GetPlayer();
	}


	StoredOverlayWidget = Cast<ADiverController>(Cast<ADiverCharacter>(player)->GetController())->VideoOverlayWidget;

	if (bIsPossessed)
	{
		UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

		if (save)
		{
			BatteryLifeInSeconds = save->divebotInfo.BatteryTime;

			if (save->divebotInfo.GraceTimeRemaninig > 0)
			{
				GetWorldTimerManager().SetTimer(Grace, this, &ADivebot::DivebotDied, save->divebotInfo.GraceTimeRemaninig);
			}
		}
	}

	FlashlightComponent->SetHiddenInGame(true);
}

void ADivebot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player && !bIsPossessed)
	{
		if (!bGoToShip)
			FollowDiver(DeltaTime);
		else
			MovementComponent->GoToShip(DeltaTime);
	}
	else if (bIsPossessed)
	{
		BatteryLifeInSeconds -= DeltaTime;
	
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, "Time Left In Batery: " + FString::SanitizeFloat(BatteryLifeInSeconds));


		float BatteryPercentage = (BatteryLifeInSeconds / BateryLifeOG);

		// Do battery changes here vvv
		if (BatteryLifeInSeconds > 100.0f)
		{
			if (StoredOverlayWidget)
				Cast<UDivebotCameraOverlayWidget>(StoredOverlayWidget)->SetBatteryLife(100);
		}
		else if (BatteryPercentage < 0.75f && NextBatteryThreshold == 75)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Purple, "75%" + FString::SanitizeFloat(BatteryPercentage));
			if (StoredOverlayWidget)
				Cast<UDivebotCameraOverlayWidget>(StoredOverlayWidget)->SetBatteryLife(NextBatteryThreshold);
			NextBatteryThreshold = 50;
		}
		else if (BatteryPercentage < 0.50f && NextBatteryThreshold == 50)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Purple, "50%" + FString::SanitizeFloat(BatteryPercentage));
			if (StoredOverlayWidget)
				Cast<UDivebotCameraOverlayWidget>(StoredOverlayWidget)->SetBatteryLife(NextBatteryThreshold);
			NextBatteryThreshold = 25;

		}
		else if (BatteryPercentage < 0.25f && NextBatteryThreshold == 25)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Purple, "25%" + FString::SanitizeFloat(BatteryPercentage));
			if (StoredOverlayWidget)
				Cast<UDivebotCameraOverlayWidget>(StoredOverlayWidget)->SetBatteryLife(NextBatteryThreshold);
			NextBatteryThreshold = 0;
		}
		else if (BatteryPercentage < 0 && NextBatteryThreshold == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Purple, "0%" + FString::SanitizeFloat(BatteryPercentage));
			if (StoredOverlayWidget)
				Cast<UDivebotCameraOverlayWidget>(StoredOverlayWidget)->SetBatteryLife(NextBatteryThreshold);
			NextBatteryThreshold = -1;

			if(!GetWorldTimerManager().IsTimerActive(Grace))
				GetWorldTimerManager().SetTimer(Grace, this, &ADivebot::DivebotDied, GracePeriodTime);
		}

	}

}

void ADivebot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

	}
}

void ADivebot::TransitionMovementStates()
{
	if (bIsPossessed)
	{
		bIsSwiming = !bIsSwiming;
		if (!bIsSwiming)
		{
			FVector dir = FirstPersonCameraComponent->GetForwardVector();
			LaunchCharacter((dir / dir.Length()) * 250, false, false);
		}
		MovementComponent->TransitionMovementStates();
	}
}

void ADivebot::SetToIsSwimming()
{
	bIsSwiming = true;
	MovementComponent->SetToIsSwimming();
	MovementComponent->ChangeEviroment(bIsSwiming);
}

void ADivebot::FollowDiver(float DeltaTime)
{
	MovementComponent->FollowDiver(DeltaTime);
	FlashlightComponent->SetHiddenInGame(true);
}


void ADivebot::PossessDivebot()
{
	if (bIsPossessed)
	{
		return;
	}

	UWorld* world = GetWorld();
	ADiverController* playerController = Cast<ADiverController>(UGameplayStatics::GetPlayerController(world, 0));

	if (playerController)
	{
		if (player)
		{
			playerController->UnPossess();

			if (Cast<ADiverCharacter>(player))
				Cast<ADiverCharacter>(player)->SetIsPossessed(false);

			playerController->Possess(this);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Divebot possession successful"));

			AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

			if (MyGameState)
			{
				FHitResult hit;

				if (FVector::Distance(GetActorLocation(), player->GetActorLocation()) < 700)
					SetActorLocation(player->GetActorLocation(), false, &hit, ETeleportType::TeleportPhysics);
			}

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Controller not valid"));
	}

	//Creates the cam overlaywidget
	if (playerController)
	{
		if (playerController->VideoOverlayWidget)
			playerController->VideoOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		else
			playerController->VideoOverlayWidget = CreateWidget<UDivebotCameraOverlayWidget>(playerController, playerController->VideoOverlayWidgetClass);

		if (StoredOverlayWidget)
			Cast<UDivebotCameraOverlayWidget>(StoredOverlayWidget)->SetBatteryLife(NextBatteryThreshold);

		if (playerController->HudWidget)
			playerController->HudWidget->SetVisibility(ESlateVisibility::Hidden);

		if (playerController->HotbarWidget)
			playerController->HotbarWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	FlashlightComponent->SetHiddenInGame(false);

	Tags.Add("Player");
	bIsPossessed = true;
	if (playerController->HeadToBoatWidget)
		playerController->HeadToBoatWidget->SetVisibility(ESlateVisibility::Visible);
	

	GetWorldTimerManager().SetTimer(popUp, this, &ADivebot::TurnOffPopUp, PopUpLifeTime);


}

void ADivebot::GetToShip(USceneComponent* spawnPoint)
{
	if (spawnPoint)
	{
		MovementComponent->boatPoint = spawnPoint;
		bGoToShip = true;
	}
}

void ADivebot::CancelGetToShip()
{
	bGoToShip = false;
	MovementComponent->boatPoint = nullptr;
}

void ADivebot::DivebotDied()
{
	if (!bIsPossessed)
	{
		return;
	}

	UWorld* world = GetWorld();
	ADiverController* playerController = Cast<ADiverController>(UGameplayStatics::GetPlayerController(world, 0));

	UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (save)
	{
		save->divebotInfo.BatteryTime = -1;
		save->divebotInfo.GraceTimeRemaninig = -1;

		UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);
	}

	if (playerController)
	{
		if (player)
		{
			AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

			if (MyGameState)
				MyGameState->RespawnPlayer();

			playerController->UnPossess();

			if (Cast<ADiverCharacter>(player))
				Cast<ADiverCharacter>(player)->SetIsPossessed(true);

			playerController->Possess(player);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player possession successful"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Controller not valid"));
	}

	//Creates the cam overlaywidget
	if (playerController)
	{
		playerController->VideoOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	Tags.Remove("Player");
	bIsPossessed = false;
}

void ADivebot::TurnOffPopUp()
{
	ADiverController* playerController = Cast<ADiverController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	playerController->HeadToBoatWidget->SetVisibility(ESlateVisibility::Hidden);
}
