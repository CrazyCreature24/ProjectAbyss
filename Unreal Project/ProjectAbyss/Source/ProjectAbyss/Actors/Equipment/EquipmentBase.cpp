// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Equipment/EquipmentBase.h"
#include "Components/BoxComponent.h"
#include "DiverCharacter.h"
#include "InteractableActor.h"
#include "InventoryComponent.h"
#include "StorageChest.h"
#include "Actors/Equipment/TrajectoryObject.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Boat.h"

// Sets default values
AEquipmentBase::AEquipmentBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize base throwing variables
	bIsThrown = false;
	WaypointIndex = 0;
	bIsCharging = false;
	ChargeSpeed = 1.0f;
	MinChargeValue = 1.0f;
	MaxChargeValue = 3.0f;
	ChargeValue = MinChargeValue;

	TrajectorySimTime = 8.0f;
	TrajectorySimFrequency = 30.0f;
	NumTragectoryMeshes = 50;

	MeshComponent->SetEnableGravity(false);
	HitboxComponent->SetEnableGravity(false);
	InteractHitboxComponent->SetEnableGravity(false);

	MeshComponent->SetSimulatePhysics(false);
	HitboxComponent->SetSimulatePhysics(false);


	SetReplicates(true);
	SetReplicateMovement(true);
}

void AEquipmentBase::AttachToBoat()
{
	//Finds the boat in the world using get all actors of class
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		this->AttachToActor(FoundActors[0], FAttachmentTransformRules::KeepWorldTransform);
		isAttachedToBoat = true;
	}
}

void AEquipmentBase::DetachFromBoat()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		isAttachedToBoat = false;
	}
}

// Called when the game starts or when spawned
void AEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipmentBase::StartThrow()
{
	if (bIsCharging)
	{
		if (Owner)
		{
			ChargeValue = MinChargeValue;
			bIsCharging = false;
			WaypointIndex = 0;
		}
	}
}

void AEquipmentBase::ContinueThrow(float DeltaTime)
{
	if (WaypointIndex >= TrajectoryPath.Num() - 1)
	{
		bIsThrown = false;
	}
	else
	{
		FVector newPos = GetActorLocation();

		FVector target = TrajectoryPath[WaypointIndex]->GetActorLocation();

		newPos = FMath::Lerp(newPos, target, DeltaTime * 15.0f);

		SetActorLocation(newPos, true);

		if ((target - newPos).Length() < 80.0f)
		{
			WaypointIndex++;
		}
	}
}

// Called every frame
void AEquipmentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If equpipment is held, set it to not hidden in game
	if (bIsInInventory == true)
	{
		if (bIsHeld == true)
		{
			SetActorHiddenInGame(false);
		}
		else if (bIsHeld == false)
		{
			SetActorHiddenInGame(true);
		}
	}


	// Charge up throw
	if (bIsCharging && ChargeValue <= MaxChargeValue)
	{
		ChargeValue += DeltaTime * ChargeSpeed;
		GEngine->AddOnScreenDebugMessage(0, 0.2f, FColor::Emerald, "Charge Value:" + FString::SanitizeFloat(ChargeValue));
	}	

	if (bIsThrown)
	{
		ContinueThrow(DeltaTime);
	}
}

void AEquipmentBase::UsePrimary()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("Primary Used"));

	if (bIsCharging == true)
	{
		bIsThrown = true;
		TurnOffTrajectory();
	}
}

void AEquipmentBase::ReleasePrimary()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("Primary Released"));
}

void AEquipmentBase::UseSecondary()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("Secondary Used"));

	if (bCanThrow)
	{
		bIsCharging = true;
	}
}

void AEquipmentBase::ReleaseSecondary()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("Secondary Released"));

	if (bCanThrow)
	{
		ChargeValue = MinChargeValue;
		bIsCharging = false;
	}
}

void AEquipmentBase::OnDropped()
{
	if (Owner)
	{
		ADiverCharacter* playerReference = Cast<ADiverCharacter>(Owner);

		if (playerReference)
		{
			UInventoryComponent* playerInventory = playerReference->InventoryComponent;

			for (int32 Index = 0; Index < playerInventory->Inventory.Num(); ++Index)
			{
				if (playerInventory->Inventory[Index] == this)
				{
					playerInventory->RemoveFromInventory(Index);
				}
			}
		}
	}
}

void AEquipmentBase::Interacted(ADiverCharacter* Player)
{
	AInteractableActor::Interacted(Player);
	// Turn off gravity when holding it
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// Adds to player inventory if slot is available. Otherwise, does nothing.
	if (Player->InventoryComponent->AddToHotbarInventory(this))
	{
		// Item has been added to the inventory (Attach to player)
		Equip(Player);
		bIsThrown = false;
		SetActorRelativeLocation(FVector(30.0f, 35.0f, 5.0f));
	}
}


void AEquipmentBase::Equip(AActor* NewOwner)
{
	// Equip the item to the owning actor
	CurrentOwner = NewOwner;
	bIsInInventory = true;
	this->SetOwner(NewOwner);
	// (This needs to be changed to a socket in the future)
	this->AttachToActor(NewOwner, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	this->SetActorEnableCollision(false);

	if (Cast<AStorageChest>(NewOwner))
		SetActorRelativeLocation(FVector(300000.0f, 100000.0f, 20.0f));
	else
		SetActorRelativeLocation(FVector(30.0f, 35.0f, 5.0f));

	if (NewOwner->ActorHasTag("Player"))
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("PlayerEquippedID"); //Nico Mod
}

void AEquipmentBase::Unequip()
{
	// Detach from Player
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	bIsInInventory = false;

	// Allow to move with gravity
	//MeshComponent->SetSimulatePhysics(true);
	SetActorEnableCollision(true);
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionProfileName("BlockAll");
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	
	//Attach to boat
	if (isAttachedToBoat)
	{
		AttachToBoat();
	}
	else
	{
		DetachFromBoat();
	}
}

void AEquipmentBase::CreateObjectPool()
{
	if (TragectoryTemplate)
	{
		NumTragectoryMeshes = TrajectorySimTime * TrajectorySimFrequency + 2;

		FActorSpawnParameters spawnParameters;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;

		UWorld* world = GetWorld();

		FVector location = GetActorLocation();
		FRotator rotation = GetActorRotation();

		for (int i = 0; i < NumTragectoryMeshes; ++i)
		{
			ATrajectoryObject* temp = Cast<ATrajectoryObject>(world->SpawnActor(TragectoryTemplate, &location, &rotation, spawnParameters));

			temp->Mesh->SetVisibility(false);

			TrajectoryPath.Add(temp);
		}
	}
}

void AEquipmentBase::ResetTragectory()
{
	// Below is stuff for tragectory. i will move to a method when it si functional
	ADiverCharacter* playerReference = Cast<ADiverCharacter>(Owner);

	FVector forwardDirection = playerReference->FirstPersonCameraComponent->GetForwardVector();

	FPredictProjectilePathResult result;
	FPredictProjectilePathParams pathParams;

	pathParams.LaunchVelocity = forwardDirection * 540 * ChargeValue;
	pathParams.SimFrequency = TrajectorySimFrequency;
	pathParams.MaxSimTime = TrajectorySimTime;
	pathParams.StartLocation = GetActorLocation();
	pathParams.DrawDebugType = EDrawDebugTrace::None;

	UGameplayStatics::PredictProjectilePath(this, pathParams, result);

	TArray<FPredictProjectilePathPointData> pathData = result.PathData;

	for (int i = 0; i < pathData.Num(); ++i)
	{
		if (i >= NumTragectoryMeshes)
			break;

		TrajectoryPath[i]->SetActorLocation(pathData[i].Location);
		TrajectoryPath[i]->Mesh->SetVisibility(true);
	}

}

void AEquipmentBase::TurnOffTrajectory()
{
	for (int i = 0; i < TrajectoryPath.Num(); ++i)
	{
		TrajectoryPath[i]->Mesh->SetVisibility(false);
	}
}

