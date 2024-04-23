// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Equipment/Seascooter.h"
#include "DiverCharacter.h"
#include "DiverMovementComponent.h"
#include <Kismet/GameplayStatics.h>

ASeascooter::ASeascooter()
{
    // Set default values
    bIsActivated = false;


    ItemName = "Sea Scooter";

    ItemDetails = FText::FromString("Hold to increase underwater speed.");

    ShopCost = 50;

}



void ASeascooter::UsePrimary()
{
    if (!bIsActivated)
    {
        // Activate Seascooter and increase player's speed
        //Cast CurrentOwner to player
        ADiverCharacter* Player = Cast<ADiverCharacter>(CurrentOwner);
        if (Player == nullptr)
        {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player is null"));
			return;
		}
        if (Player->MyMovementComponent == nullptr)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player's movement component is null"));
            return;
        }


        Player->MyMovementComponent->SetSpeed(Player->MyMovementComponent->GetSpeed() + SpeedIncrease);
        
        bIsActivated = true;

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Seascooter Activated"));
    }
    else
    {
		// Deactivate Seascooter and reset player's speed
		//Cast CurrentOwner to player
		ADiverCharacter* Player = Cast<ADiverCharacter>(CurrentOwner);
		Player->MyMovementComponent->ResetSpeed();

		bIsActivated = false;

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Seascooter Deactivated"));
	}
}

void ASeascooter::ReleasePrimaryTest()
{
    if (bIsActivated)
    {
		// Deactivate Seascooter and reset player's speed
		//Cast CurrentOwner to player
		ADiverCharacter* Player = Cast<ADiverCharacter>(CurrentOwner);
		Player->MyMovementComponent->ResetSpeed();

		bIsActivated = false;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Seascooter Deactivated"));
	}

}

void ASeascooter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //If this equipment is no longer held, it'll release the primary, but only ticks once.
    if (bIsActivated && !bIsHeld)
    {
		ReleasePrimaryTest();
	}

    if (bIsActivated)
    {
        ADiverCharacter* Player = Cast<ADiverCharacter>(CurrentOwner);

        // Makes sound while activated
        if (Player == nullptr)
        {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player is null"));
            bIsActivated = false;
			return;
		}
        UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, Player->GetActorLocation(), 0.0f, 2000.0f);
        
        MakeNoise(1.0f, Player, Player->GetActorLocation(), 2000.0f);
    }
}

void ASeascooter::Equip(AActor* NewOwner)
{
    Super::Equip(NewOwner);


    //Cast CurrentOwner to player
    ADiverCharacter* Player = Cast<ADiverCharacter>(CurrentOwner);
}
