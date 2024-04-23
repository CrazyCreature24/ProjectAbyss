// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Pawns/DiverCharacter.h"
#include "Components/CapsuleComponent.h"
#include "DiverMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractComponent.h"
#include "PhotographyCamera.h"
#include "HealthStateComponent.h"
#include "Components/StressComponent.h"
#include "Components/SpotLightComponent.h"
#include "../../../../Plugins/GameFrwkSessionsPlugin/Source/GameFrwkSessionsPlugin/OnlineGameInstance.h"
#include "InventoryComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "MainGameState.h"
#include "Boat.h"
#include "HUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectAbyssSaveGame.h"
#include "Components/AudioComponent.h"
#include "Animation/AnimSequence.h"
#include <DiverController.h>
#include "Components/SkeletalMeshComponent.h"



// Sets default values
ADiverCharacter::ADiverCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UDiverMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));

	StressComponent = CreateDefaultSubobject<UStressComponent>("StressComponent");

	HealthComponent = CreateDefaultSubobject<UHealthStateComponent>("HealthComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->MaxInventorySlots = 4;

	CamInventoryComponent = CreateDefaultSubobject<UInventoryComponent>("CamInventoryComponent");
	CamInventoryComponent->MaxInventorySlots = 1;

	MyMovementComponent = Cast<UDiverMovementComponent>(GetMovementComponent());

	VOIPTalkerComponent = CreateDefaultSubobject<UVOIPTalker>("VoiceComponent");

	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComponent"));
	FlashlightComponent->SetupAttachment(FirstPersonCameraComponent);
	FlashlightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	FlashlightComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	FlashlightComponent->Intensity = 5000.0f;
	FlashlightComponent->OuterConeAngle = 30.0f;

	BreathingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BreathingAudioComponent"));

	RunAnim = CreateDefaultSubobject<UAnimSequence>(TEXT("RunAnim"));

	DeathMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DeathMesh"));
	DeathMesh->SetupAttachment(RootComponent);
	DeathMesh->SetVisibility(false);
	
	Tags.Add("Player");
}

// Called when the game starts or when spawned
void ADiverCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	InventoryComponent->Inventory.SetNum(InventoryComponent->MaxInventorySlots);

	CamInventoryComponent->Inventory.SetNum(CamInventoryComponent->MaxInventorySlots);

	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{
		MyGameState->SetPlayer(this);
	}

	UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
	FHitResult hit;

	if (save && MyGameState)
	{
		if (GetWorld()->GetMapName().Contains("OverWorld")) // TODO: Need to make Blueprint method to check if this level is open
		{
			if (save->HasPlayerDied)
			{
				if (save->divebotInfo.BatteryTime > 0 || save->divebotInfo.GraceTimeRemaninig > 0)
				{
					MyGameState->OnLevelTransferSpawnAsDivebot();
				}
			}
			else if (save->bHasEnteredCave)
			{
				save->bHasEnteredCave = false;
				SetActorLocationAndRotation(save->CavePointLoc, save->CavePointRot, false, &hit, ETeleportType::TeleportPhysics);
			}
		}
	}

	UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);
	//SetupCamera();
}

// Called every frame
void ADiverCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	if (isCameraSetup == false)
	{
		SetupCamera();
		isCameraSetup = true;
	}

	if (bIsFirstTick)
	{
		UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
		if (save && save->HasPlayerDied && save->HasPlayerRespawn == false)
		{
			save->HasPlayerRespawn = true;
			save->HasPlayerDied = false;

			AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

			if(MyGameState && MyGameState->GetBoat())
				SetActorLocation((MyGameState->GetBoat()->GetRespawnPointLocation()));

			UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);
		}
	}

	//To play footstep sounds at proper intervals
	if (MyMovementComponent->bIsMoving && MyMovementComponent->MovementMode == EMovementMode::MOVE_Walking && !FootstepIntervalHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(FootstepIntervalHandle, this, &ADiverCharacter::PlayFootsteps, 0.4f, true, 0.15);
	}
	else if (!(MyMovementComponent->bIsMoving) || MyMovementComponent->MovementMode != EMovementMode::MOVE_Walking)
	{
		GetWorldTimerManager().ClearTimer(FootstepIntervalHandle);
		FootstepIntervalHandle.Invalidate();
	}

	if (MyMovementComponent->MovementMode == EMovementMode::MOVE_Swimming && BreathingAudioComponent->IsPlaying() == false)
	{
		BreathingAudioComponent->Play();
	}
	else if (MyMovementComponent->MovementMode != EMovementMode::MOVE_Swimming)
	{
		BreathingAudioComponent->Stop();
	}

	bIsFirstTick = false;

	//if(!bSoundDecoyPlaying)
		//MakeNoise(1.0f, this, GetActorLocation(), 2000.0f);
}

// Called to bind functionality to input
void ADiverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADiverCharacter::TransitionMovementStates()
{
	bIsSwiming = !bIsSwiming;
	if (!bIsSwiming)
	{
		FVector dir = GetLastMovementInputVector() / GetLastMovementInputVector().Length();
		LaunchCharacter(dir * 100, false, false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExitWaterSound, GetActorLocation());
	}
	else if (bIsSwiming)
	{
		// Put sound for jumping in water
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnterWaterSound, GetActorLocation());
		
	}
	MyMovementComponent->TransitionMovementStates();
	StressComponent->ChangeEviroment(bIsSwiming);
}

void ADiverCharacter::SetToIsSwimming()
{
	bIsSwiming = true;
	MyMovementComponent->SetToIsSwimming();
	StressComponent->ChangeEviroment(bIsSwiming);
}

void ADiverCharacter::SetupCamera()
{
	if (CameraClass)
	{
		if (CameraActor == nullptr)
		{
			CameraActor = GetWorld()->SpawnActor<APhotographyCamera>(CameraClass, GetActorLocation(), GetActorRotation());
			CameraActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			CameraActor->SetActorRelativeLocation(FVector(0.f, 0.f, 60.f));

			// Set owner of camera to player
			CameraActor->Equip(this);
			//Cast to controller
			ADiverController* MyController = Cast<ADiverController>(GetController());
			if (MyController)
			{
				MyController->HotbarCamSlot();
			}

			CamInventoryComponent->AddCamera(CameraActor);
		}
	}
}

void ADiverCharacter::PlayFootsteps()
{
	UGameplayStatics::PlaySoundAtLocation(this, Footsteps, GetActorLocation(), 2.0f);
}

void ADiverCharacter::SetIsPossessed(bool b)
{
	bIsPossessed = b;

	myCrakedVisorWdiget->SetWidgetVisible(b);

}

void ADiverCharacter::SwapPlayerToDeathMesh(bool bIsDead)
{
	GetMesh()->SetVisibility(!bIsDead);
	DeathMesh->SetVisibility(bIsDead);
}

void ADiverCharacter::OnRespawn_Implementation()
{

}

/*
ChangeLog:
-----------------------------
Date:	 	Oct 12 2023
Name:		Maxime Paradis
Summary:	Created player character with relvent components
-----------------------------
Date:	 	Oct 19 2023
Name:		Maxime Paradis
Summary:	Commented relevent code
-----------------------------
Date:	 	Jan 31 2024
Name:		Maxime Paradis
Summary:	Added respawning to player
-----------------------------
Date:	 	Feb 15 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	Adding footsteps
-----------------------------
*/