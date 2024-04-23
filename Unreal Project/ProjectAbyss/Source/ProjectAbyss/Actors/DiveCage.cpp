// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/DiveCage.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Boat.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"


ADiveCage::ADiveCage()
{
	

	//Setup cage door mesh
	CageDoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cage Door Mesh"));
	CageDoorMeshComponent->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Cage Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 0, 50));

	InsideVolume = CreateDefaultSubobject<UBoxComponent>("Inside Volume");
	InsideVolume->SetupAttachment(RootComponent);
	InsideVolume->SetCollisionProfileName("NoCollision");
	InsideVolume->SetGenerateOverlapEvents(true);
	InsideVolume->OnComponentBeginOverlap.AddDynamic(this, &ADiveCage::OnOverlapBegin);
	InsideVolume->OnComponentEndOverlap.AddDynamic(this, &ADiveCage::OnOverlapEnd);

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture"));
	Capture->SetupAttachment(SpringArm);

	InteractHitboxComponent->SetupAttachment(CageDoorMeshComponent);

	Tags.Add("Cage");
}

void ADiveCage::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();

	//Cast to boat
	//Find dive cage using get all actors of class
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), FoundActors);

	//Set myBoat to the first boat found
	myBoat = Cast<ABoat>(FoundActors[0]);
}

void ADiveCage::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADiverCharacter* p = Cast<ADiverCharacter>(OtherActor);
	if (p)
	{
		MyPlayer = p;
	}
}

void ADiveCage::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MyPlayer == OtherActor)
	{
		MyPlayer = nullptr;
	}
}

void ADiveCage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RaycastToGround();
}

void ADiveCage::Interacted(ADiverCharacter* Player)
{
	AInteractableActor::Interacted(Player);

	//Toggle cage door
	ToggleCageDoor();
}

float ADiveCage::GetDistanceToGround()
{
	return DistanceToGround;
}

void ADiveCage::SetDistanceToGround(float NewDistance)
{
	DistanceToGround = NewDistance;
}

void ADiveCage::LowerCage()
{
	bIsCageLowered = true;

	//If cage is not at the bottom, lower it by 3 units
	if (myBoat->GetIsBoatMoving() == false)
	{
		if (bIsLocked == true && bIsDoorOpen == false)
		{
			if (DistanceToGround > 1)
			{
				SetActorLocation(GetActorLocation() - FVector(0, 0, CageMoveSpeed), true);
				if (MyPlayer)
					MyPlayer->SetActorLocation(MyPlayer->GetActorLocation() + FVector(0, 0, -CageMoveSpeed));

			}
		}

	}

}

void ADiveCage::RaiseCage()
{
	
	if (myBoat->GetIsBoatMoving() == false)
	{
		if (bIsLocked == true && bIsDoorOpen == false)
		{
			if (GetActorLocation().Z < OriginalLocation.Z)
			{
				SetActorLocation(GetActorLocation() + FVector(0, 0, CageMoveSpeed), true);
				if (MyPlayer)
					MyPlayer->SetActorLocation(MyPlayer->GetActorLocation() + FVector(0, 0, CageMoveSpeed));

			}
		}
	}
}

void ADiveCage::ToggleCageDoor()
{
	//If cage is not at the bottom, lower it by 3 units
	if (myBoat->GetIsBoatMoving() == false)
	{
		bIsDoorOpen = !bIsDoorOpen;

		//If the door is open, start opening it. If it is closed, start closing it
		if (bIsDoorOpen)
		{
			// Start opening the door
			GetWorldTimerManager().SetTimer(DoorMoveTimerHandle, this, &ADiveCage::OpenCageDoor, 0.01f, true, 0.0f);
		}
		else
		{
			// Start closing the door
			GetWorldTimerManager().SetTimer(DoorMoveTimerHandle, this, &ADiveCage::CloseCageDoor, 0.01f, true, 0.0f);
		}
	}
}

void ADiveCage::OpenCageDoor()
{
	//If rotation of the door is less than 90 degrees, rotate it by 1 degree
	if (CageDoorMeshComponent->GetRelativeRotation().Yaw < 90.0f)
	{
		FRotator NewRotation = CageDoorMeshComponent->GetRelativeRotation();
		NewRotation.Yaw += 1.0f;

		CageDoorMeshComponent->SetRelativeRotation(NewRotation);
	}
	else
	{
		//Stop the timer
		GetWorldTimerManager().ClearTimer(DoorMoveTimerHandle);
	}


}

void ADiveCage::CloseCageDoor()
{
	//If rotation of the door is greater than 0 degrees, rotate it by -1 degree
	if (CageDoorMeshComponent->GetRelativeRotation().Yaw > 0.0f)
	{
		FRotator NewRotation = CageDoorMeshComponent->GetRelativeRotation();
		NewRotation.Yaw -= 1.0f;

		CageDoorMeshComponent->SetRelativeRotation(NewRotation);
	}
	else
	{
		//Stop the timer
		GetWorldTimerManager().ClearTimer(DoorMoveTimerHandle);
	}

}

void ADiveCage::ToggleLockCageDoor()
{
	//If the cage isnt at its default location
	//if (GetActorLocation().Z == OriginalLocation.Z)
	//{

		if (bIsLocked)
		{
			bIsLocked = false;
		}
		else
		{
			bIsLocked = true;
			
			TSet<AActor*> p;
			InsideVolume->GetOverlappingActors(p, PlayerSubclass);
			if (p.Num())
			{
				MyPlayer = Cast<ADiverCharacter>(p.Array()[0]);
			}
		}

	//}
	
}

void ADiveCage::RaycastToGround()
{
	//Constant check to measure distance to ground floor using a raycast
	FHitResult HitResult;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0, 0, MaxCageDepthMeters * 100);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->ActorHasTag("Ground"))
        {
			DistanceToGround = HitResult.Distance/100;
			//Clamp distance to multiply by ten, then round, divide by ten.
		 DistanceToGround = FMath::RoundToFloat(FMath::Clamp(DistanceToGround, 0.0f, MaxCageDepthMeters) * 10) / 10;


			//Debug print distance to ground
			GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Red, FString::Printf(TEXT("Cage distance to ground: %f"), DistanceToGround));

            DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Green, false, 0.1f, 0, 1);
        }
		else
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.1f, 0, 1);

		}
       
	}

}
