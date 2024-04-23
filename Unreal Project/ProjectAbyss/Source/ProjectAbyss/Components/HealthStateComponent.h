// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthStateComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		bool bIsTemp = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		float lengthOfState = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		float CurrentengthOfState = lengthOfState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		float StressAmount = 10;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UHealthStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<FDamageState> MyDamageStates;
	FTimerHandle IFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer Var")
	float TimeForIFrames = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer Var")
	float StressAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer Var")
	float DeathDelay = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer Var")
	float HitResponseSpeedMultiplier = 1.43f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer Var")
	float KnockbackForce = 800.f;

	bool bIsDead = false;
	FTimerHandle DeathDelayHandle;

	
	class USoundBase* MaskCrackSoundCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* MaskCrackSound1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* MaskCrackSound2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* SwitchToDivebotSound;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(bool bIsPerminent, float lengthOfState, int amountOfStates, FVector fishPos = {0,0,0}, float KnockBack = 400);
		
	void DEBUG_KILLPLAYER();

	UFUNCTION(BlueprintCallable, Category = "PlayerDeath")
	void AttemptToRecover();

	UFUNCTION(BlueprintCallable, Category = "PlayerDeath")
	void Respawn();

	inline TArray<FDamageState> GetHPSaveData() { return MyDamageStates; }

	inline void SetHPSaveData(TArray<FDamageState> states) 
	{ 
		MyDamageStates.Empty();
		if(states.Num() < 4) MyDamageStates = states; 
	}

	UFUNCTION(BlueprintNativeEvent, Category = "PlayerDeath")
	void OnDeath();

	inline bool GetIsDead() 
	{ return bIsDead; }

	void Reset();
	FORCEINLINE int GetNumStates() { return MyDamageStates.Num(); }

};

/*
Date:	 	March 5th 2024
Name :		Alexander McTernan
Summary :	Added Mask cracking sound functionality and the Switch to Divebot sound.
---------------------------- -
Date:	 	April 4th
Name:		Alexander McTernan
Summary:	Added swap player mesh on death.
-----------------------------
*/