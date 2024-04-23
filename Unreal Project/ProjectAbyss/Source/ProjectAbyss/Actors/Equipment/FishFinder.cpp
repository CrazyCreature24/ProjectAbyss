// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Equipment/FishFinder.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include <Creature.h>
#include "FishFinderWidget.h"


AFishFinder::AFishFinder()
{
    // Create the box component for hit detection
    Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
    Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision initially
    Hitbox->SetupAttachment(RootComponent);
    CurrentSize = FVector::ZeroVector;
    Hitbox->SetBoxExtent(CurrentSize);

    // Create the fish finder widget component
    FishFinderWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FishFinderWidget"));
    FishFinderWidget->SetupAttachment(Hitbox);
    FishFinderWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    FishFinderWidget->SetWidgetSpace(EWidgetSpace::World);
    FishFinderWidget->SetDrawSize(FVector2D(100.0f, 100.0f));


    // Set default values
    GrowthRate = 1000.0f; // Change this value as needed
    MaxSize = FVector(100.0f, 100.0f, 100.0f); // Change this value agfbvs needed
    ScaleInterval = 0.01f; // Change this value as needed
    InitialDelay = 1.0f; // Change this value as needed

    ItemName = "Fish Finder"; //Used for Equipment Loading and Saving

    ItemDetails = FText::FromString("Use to highlight all fish nearby. Displays number of fish nearby on the item screen.");

    Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AFishFinder::OnHitboxOverlapBegin);

    ShopCost = 50;
}

void AFishFinder::UsePrimary()
{
    // Enable collision after some delay
    GetWorldTimerManager().SetTimer(EnableCollisionTimerHandle, this, &AFishFinder::EnableCollision, InitialDelay, false);
}

void AFishFinder::ReleasePrimary()
{
}

void AFishFinder::UseSecondary()
{
}

void AFishFinder::ReleaseSecondary()
{
}

void AFishFinder::Equip(AActor* NewOwner)
{
    Super::Equip(NewOwner);
    this->SetActorEnableCollision(true);
    InteractHitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFishFinder::BeginPlay()
{
    Super::BeginPlay();

}

void AFishFinder::EnableCollision()
{
    if (Hitbox)
    {
        Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Enable collision
        GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &AFishFinder::ScaleHitbox, ScaleInterval, true);
        FishCount = 0;
        if (FishFinderWidget)
        {
            Cast<UFishFinderWidget>(FishFinderWidget->GetUserWidgetObject())->SetFishCount(FishCount);
        }
    }
}

void AFishFinder::ScaleHitbox()
{
    //If current size is less than max size
    if (CurrentSize.X < MaxSize.X && CurrentSize.Y < MaxSize.Y && CurrentSize.Z < MaxSize.Z)
    {
		//Increase size
		CurrentSize += FVector(GrowthRate * GetWorld()->GetDeltaSeconds());
		CurrentSize.X = FMath::Clamp(CurrentSize.X, 0.0f, MaxSize.X);
		CurrentSize.Y = FMath::Clamp(CurrentSize.Y, 0.0f, MaxSize.Y);
		CurrentSize.Z = FMath::Clamp(CurrentSize.Z, 0.0f, MaxSize.Z);

		//Set the new size
		Hitbox->SetBoxExtent(CurrentSize);
	}
    else
    {
		//Stop scaling
		GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
        ////Disable collision
        Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        //Reset size
        CurrentSize = FVector::ZeroVector;
        Hitbox->SetBoxExtent(CurrentSize);
        ScannedFish.Empty();
	}
}

void AFishFinder::OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    //If other actor has tag Creature
    if (OtherActor->ActorHasTag("Creature"))
	{
        //Casts to ACreature and spawns scanned widget
        ACreature* Creature = Cast<ACreature>(OtherActor);

        if (Creature && !ScannedFish.Contains(Creature))
		{
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Creature Found"));
            Creature->ShowScannedWidget();

            ScannedFish.Add(Creature);

            FishCount++;

            if (FishFinderWidget)
            {
				Cast<UFishFinderWidget>(FishFinderWidget->GetUserWidgetObject())->SetFishCount(FishCount);
            }

		}
	}

}