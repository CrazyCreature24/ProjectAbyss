// Copyright Astralwater Interactive. All Rights Reserved.


#include "HealthStateComponent.h"
#include "StressComponent.h"
#include "Gamemodes/ProjectAbyssGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Boat.h"
#include "Divebot.h"
#include "DiverCharacter.h"
#include "DiverMovementComponent.h"
#include "DiverController.h"
#include "MainGameState.h"
#include "ProjectAbyssSaveGame.h"
#include "DiverController.h"
#include "PhotographyCamera.h"
#include "CameraViewfinder.h"


// Sets default values for this component's properties
UHealthStateComponent::UHealthStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



	

	// ...
}


// Called when the game starts
void UHealthStateComponent::BeginPlay()
{
	Super::BeginPlay();

	MaskCrackSoundCurrent = MaskCrackSound1;
	// ...
	
}


// Called every frame
void UHealthStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Orange,"Current HeathStates: " + FString::FromInt(MyDamageStates.Num()));
	// ...

	if (!bIsDead)
	{

		TArray<int> IndexesToRemove;

		for (int i = 0; i < MyDamageStates.Num(); i++)
		{
			if (MyDamageStates[i].bIsTemp)
				MyDamageStates[i].CurrentengthOfState -= DeltaTime;

			if (MyDamageStates[i].CurrentengthOfState <= 0)
			{
				MyDamageStates[i].lengthOfState -= DeltaTime;
				if (MyDamageStates[i].lengthOfState <= 0)
				{
					IndexesToRemove.Add(i);
				}
			}
		}

		for (int i : IndexesToRemove)
		{
			MyDamageStates.RemoveAt(i);
		}
	}

}

void UHealthStateComponent::TakeDamage(bool bIsPerminent, float lengthOfState, int amountOfStates, FVector fishPos, float KnockBack)
{
	if (bIsDead)
		return;

	ADiverCharacter* parent = Cast<ADiverCharacter>(GetOwner());

	if (parent)
	{
		parent->MyMovementComponent->HitSpeed(HitResponseSpeedMultiplier);
		parent->LaunchCharacter((parent->GetActorLocation() - fishPos) / (fishPos - parent->GetActorLocation()).Length() * KnockBack, true, true); // todo

	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(IFrames))
	{
		FDamageState newState;
		GetWorld()->GetTimerManager().SetTimer(IFrames, TimeForIFrames, false);
		for (int i = 0; i < amountOfStates; i++)
		{

			newState.bIsTemp = !bIsPerminent;
			newState.lengthOfState = lengthOfState;

			MyDamageStates.Add(newState);
			GetOwner()->GetComponentByClass<UStressComponent>()->AddFlatStress(10);
		}

		for (int i = 0; i < MyDamageStates.Num(); i++)
		{
			if (MyDamageStates[i].bIsTemp)
			{
				MyDamageStates[i].CurrentengthOfState = lengthOfState + newState.lengthOfState * i;
			}
		}

		if (MyDamageStates.Num() >= 4)
		{
			bIsDead = true;
			GetWorld()->GetTimerManager().SetTimer(DeathDelayHandle, this, &UHealthStateComponent::OnDeath, DeathDelay);
			GetOwner()->bBlockInput = true;
		}

		// Mask crack sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MaskCrackSoundCurrent, GetOwner()->GetActorLocation());

		if (MaskCrackSoundCurrent == MaskCrackSound1)
			MaskCrackSoundCurrent = MaskCrackSound2;
		else
			MaskCrackSoundCurrent = MaskCrackSound1;
	}
}

void UHealthStateComponent::DEBUG_KILLPLAYER()
{
	bIsDead = true;
	OnDeath();
	GetOwner()->bBlockInput = true;
}

void UHealthStateComponent::AttemptToRecover()
{
	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{
		ADiverCharacter* diver = Cast<ADiverCharacter>(GetOwner());
		if (diver)
		{
			if (MyGameState->GetBot())
			{
				MyGameState->GetBot()->PossessDivebot();
				diver->SwapPlayerToDeathMesh(true);
			}
		}
	}
}

void UHealthStateComponent::Respawn()
{
	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{

		ADiverCharacter* diver = Cast<ADiverCharacter>(GetOwner());
		if (diver)
		{
			diver->OnRespawn();
			diver->SwapPlayerToDeathMesh(false);
			MyGameState->RespawnPlayer();
		}
	}
}

void UHealthStateComponent::Reset()
{
	MyDamageStates.Empty();
	bIsDead = false;
}

void UHealthStateComponent::OnDeath_Implementation()
{
	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (Cast<ADiverCharacter>(GetOwner())->CameraActor)
	{
		if (Cast<ADiverCharacter>(GetOwner())->CameraActor->GetAimingMode())
		{
			Cast<ADiverCharacter>(GetOwner())->CameraActor->ReleaseSecondary();
		}

		Cast<ADiverCharacter>(GetOwner())->CameraActor->ReleasePrimary();
	}

	if (save)
	{
		save->HasPlayerDied = true;
		save->HasPlayerRespawn = false;
		UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);
	}

	if (MyGameState)
	{
		MyGameState->PlayerDied();



		// Death switch to Divebot sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwitchToDivebotSound, GetOwner()->GetActorLocation());
	}


	//Respawn();
	AttemptToRecover();
	//Todo add divebot stuff
}


