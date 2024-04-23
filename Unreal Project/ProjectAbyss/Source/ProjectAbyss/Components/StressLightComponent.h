// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "StressLightComponent.generated.h"

UENUM(BlueprintType)
enum class ELightType : uint8
{
	Decrease UMETA(DisplayName = "Decrease Stress"),
	Slow UMETA(DisplayName = "Slow Stress"),
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class PROJECTABYSS_API UStressLightComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStressLightComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ELightType MyLightType = ELightType::Decrease;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bShouldTick = false;

	TArray<class UStressComponent*> myPlayers;

	class UPointLightComponent* myLight;

	bool bIsStillBright = true;

	float OGIntensity;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionSphere")
		class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Designer Variables")
		float StressDecreasedPersecond = 0.5;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Designer Variables")
		float PercentageLightDecrease = 1;
};
