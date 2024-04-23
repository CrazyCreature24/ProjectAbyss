// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StressComponent.generated.h"

UENUM(BlueprintType)
enum class EStressThreshold : uint8 {
	ECalm = 0		UMETA(DisplayName = "Calm"),
	EAgitated = 1	UMETA(DisplayName = "Agitated"),
	EStressed = 2    UMETA(DisplayName = "Stressed"),
	EPanicked = 3	UMETA(DisplayName = "Panicked")
};

USTRUCT(BlueprintType)
struct FStateInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
	EStressThreshold MyStressState = EStressThreshold::ECalm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
	float ThershHoldValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
	FString MyName;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UStressComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStressComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeEviroment(bool  IsInWater);
	void AddFlatStress(float flatPercentalAmount) { CurrentStressPercentage += flatPercentalAmount; }
	void TickStress();

	bool SetStressDecrease(float Value);

	float CurrentStressPercentage = 0.0f;
	int CurrentDebthMultiplier = 0;
	bool bIsInWater = false;
	FTimerHandle StressTimerHandle;

	bool bIsStressIncreasing = true;
	bool bCanDecrease = false;
	float DecreaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true"))
		float StressIncreaseRate = 0.25f; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|Negatives", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
		float DebthMultiplier = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|Negatives", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
		float DebthMultiplierThreashHold = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|Positives", meta = (AllowPrivateAccess = "true", ClampMax = "0"))
		float BrigtLightDecreaseMultiplier = -2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|Positives", meta = (AllowPrivateAccess = "true", ClampMax = "0"))
		float SurfaceDecreaseMultiplier = -10.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|Stats", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
		/// <summary> The amount of time before stress loop starts </summary>
		float StressGracePeriod = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|Stats", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
		/// <summary> The amount of seconds inbetween each increase of stress </summary>
		float TickRate = 1.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables|States", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
		TArray<FStateInfo> MyStates;

		int CurrentStressStateIndex = 0;
};
