// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Pawns/Boat.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <DiverCharacter.h>
#include "DiverController.h"
#include "Kismet/GameplayStatics.h"
#include "DiveCage.h"
#include "BoatWheel.h"
#include "MainGameState.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Subsystems/SaveLoadSubsystem.h"
#include "Components/BoxComponent.h"
#include "Divebot.h"
#include "EquipmentBase.h"
#include "Actors/Radio.h"
#include "MediaSoundComponent.h"
#include "Components/AudioComponent.h"

ABoat::ABoat()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Box"));
	RootComponent = RootBox;

	//Mesh 
	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	BoatMesh->SetupAttachment(RootBox);
	BoatMesh->SetCollisionProfileName(TEXT("Pawn"));
	BoatMesh->SetSimulatePhysics(true);

	// Create and attach the spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoatMesh);
	SpringArm->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f)); // Set spring arm rotation
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // Set spring arm location
	SpringArm->TargetArmLength = 1000.0f; // Set spring arm length
	SpringArm->bDoCollisionTest = false; // Disable spring arm collision test


	SpringArm->bEnableCameraLag = true; // Enable camera lag
	SpringArm->CameraLagSpeed = 25.0f; // Set camera lag speed
	SpringArm->bEnableCameraRotationLag = true; // Enable camera rotation lag

	// Create and attach the camera
	BoatCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BoatCamera"));
	BoatCamera->SetupAttachment(SpringArm);
	BoatCamera->bUsePawnControlRotation = false; // Disable pawn control over camera rotation

	respawnPoint = CreateDefaultSubobject<USceneComponent>("RespawnPoint");
	respawnPoint->SetupAttachment(BoatMesh);

	DivebotSpawnPoint = CreateDefaultSubobject<USceneComponent>("DivebotSpawnPoint");
	DivebotSpawnPoint->SetupAttachment(BoatMesh);

	//Trigger box used to recover lost stuff
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABoat::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABoat::OnOverlapEnd);

	// Sound Component
	BoatSounds = CreateDefaultSubobject<UAudioComponent>(TEXT("BoatSounds"));
	BoatSounds->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();

	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{
		MyGameState->SetBoat(this);
	}

	FString MapName = GetWorld()->GetMapName(); 
	//this is so the boat in the menu is not in some random spot when loaded.
	//if (GetWorld()->GetMapName() != L"UEDPIE_0_MainMenuMap" || GetWorld()->GetMapName() != L"UEDPIE_0_TutorialLevel") //How the heck was i suppose this was the map name
	//if (GetWorld()->GetMapName() == L"UEDPIE_0_OverWorld" ) //How the heck was i suppose this was the map name
	//{
	//	UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>()->LoadBoatState();
	//}

	//FString MapName = GetWorld()->GetMapName(); 
	////this is so the boat in the menu is not in some random spot when loaded.
	////if (GetWorld()->GetMapName() != L"UEDPIE_0_MainMenuMap" || GetWorld()->GetMapName() != L"UEDPIE_0_TutorialLevel") //How the heck was i suppose this was the map name
	//if (GetWorld()->GetMapName() == L"UEDPIE_0_OverWorld" ) //How the heck was i suppose this was the map name
	//{
	//	UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>()->LoadBoatState(this);
	//}

	TArray<AActor*> temparray;
	
	//Get PC terminal actor from world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerBox::StaticClass(), temparray);

	for (AActor* tempactor : temparray)
	{
		if (tempactor->ActorHasTag("MapBounds"))
		{
			StoredMapBox = Cast<ATriggerBox>(tempactor);
		}

	}

	if (StoredMapBox)
	{
		StoredMapBox->GetActorBounds(false, MapBoxOrigin, MapBoxExtent);
	}
	
	if (BoatSounds)
	{
		BoatSounds->Play();
	}

}

// Called every frame
void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentSpeed = TargetVelocity.Y != 0 ? FMath::Lerp(CurrentSpeed, TargetVelocity.Y, DeltaTime * Acceleration) : FMath::Lerp(CurrentSpeed, TargetVelocity.Y, DeltaTime * Deceleration);

	FHitResult hit;

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * CurrentSpeed * DeltaTime, true, &hit, ETeleportType::ResetPhysics);

	CurrentTurnSpeed = TargetVelocity.X != 0 ? FMath::Lerp(CurrentTurnSpeed, TargetVelocity.X, 0.5 * DeltaTime) : FMath::Lerp(CurrentTurnSpeed, TargetVelocity.X, 0.9 * DeltaTime);

	AddActorWorldRotation(FRotator(0, CurrentTurnSpeed * DeltaTime, 0), true);

}

void ABoat::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<ADivebot>(OtherActor))
	{
		ADivebot* divebot = Cast<ADivebot>(OtherActor);

		if (divebot->bIsPossessed)
		{
			AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

			if (MyGameState)
			{
				divebot->GetController()->Possess(MyGameState->GetPlayer());
				MyGameState->RespawnPlayer();
			}
		}
	}
	if (Cast<AEquipmentBase>(OtherActor))
	{
		 AEquipmentBase* equipment = Cast<AEquipmentBase>(OtherActor);

		 equipment->isAttachedToBoat = true;
	}
}

void ABoat::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AEquipmentBase>(OtherActor))
	{
		AEquipmentBase* equipment = Cast<AEquipmentBase>(OtherActor);

		equipment->isAttachedToBoat = false;
	}
}


//TODO
// Called to bind functionality to input
void ABoat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (GetLocalRole() == ROLE_Authority || GetLocalRole() == ROLE_SimulatedProxy) // allow server and client to setup the inputs
	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
		{
			//Move boat

			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABoat::HandleMovement);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABoat::HandleMovement);


			EnhancedInputComponent->BindAction(UnPossessBoatAction, ETriggerEvent::Triggered, this, &ABoat::UnPossessBoat);
		}
	}
}

void ABoat::HandleMovement(const FInputActionValue& Value)
{
	FVector2D moveInput = Value.Get<FVector2D>();

	//Calculate the target speed
	TargetVelocity.Y = moveInput.Y > 0 ? MaxSpeed * moveInput.Y : (MaxSpeed / 2) * moveInput.Y;

	// Calculate the target rotation speed
	TargetVelocity.X = TurnSpeed * moveInput.X;
}


void ABoat::DecelerateBoat()
{
	bIsDecelerating = true;
	if (CurrentSpeed > 0.0f)
	{
		// If current speed is < 5.0f , set it to 0.0f
		if (CurrentSpeed < 5.0f)
		{
			CurrentSpeed = 0.0f;
		}
		// Gradually reduce the speed
		CurrentSpeed = FMath::FInterpTo(CurrentSpeed, 0.0f, GetWorld()->GetDeltaSeconds(), Deceleration);

		// Scale the input value by the movement speed
		FVector ForwardVector = GetActorForwardVector();

		// Move the boat
		AddActorWorldOffset(ForwardVector * CurrentSpeed * GetWorld()->GetDeltaSeconds() * BoatSpeedMultiplier, true);

		// Output current speed as debug msg
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Current Speed: %f"), CurrentSpeed));
	}
}

void ABoat::PossessBoat(ADiverCharacter* Player)
{
	// Get the player controller by casting the player pawn's controller
	ADiverController* PlayerController = Cast<ADiverController>(Player->GetController());

	if (PlayerController)
	{
		CurrentPlayerCharacter = Player;

		// Unpossess the current pawn
		PlayerController->UnPossess();

		// Set the controller to nullptr after unpossessing
		Player->Controller = nullptr;

		// Possess the boat pawn
		PlayerController->OnPossess(this);

		Player->SetIsPossessed(false);

		TArray<AActor*> temparray;

		//Get PC terminal actor from world
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARadio::StaticClass(), temparray);

		if (temparray.IsEmpty() == false)
		{
			ARadio* radio = Cast<ARadio>(temparray[0]);

			//radio->SoundComp->Set
		}

	}

	//Debug message to show who is possessing the boat
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Possessed by: %s"), *Player->GetName()));
}

//TODO
void ABoat::UnPossessBoat()
{
	ADiverController* PlayerController = Cast<ADiverController>(this->GetController());
	if (PlayerController)
	{

		// Unpossess the boat pawn
		PlayerController->UnPossess();

		// Possess the player pawn that was previously possessed
		PlayerController->OnPossess(CurrentPlayerCharacter);

		CurrentPlayerCharacter->SetIsPossessed(true);

		TargetVelocity.X = 0;
	}
}

