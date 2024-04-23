// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipmentBase.h"
#include "Seascooter.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTABYSS_API ASeascooter : public AEquipmentBase
{
	GENERATED_BODY()

public:
	ASeascooter();

	//Equip override
	virtual void Equip(AActor* NewOwner) override;
protected:
	virtual void UsePrimary() override;
	void ReleasePrimaryTest();

	//Tick
	virtual void Tick(float DeltaTime) override;

	//Camera Shake
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer", meta = (AllowPrivate))
	TSubclassOf<UCameraShakeBase> CameraShake;

private:
	bool bIsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer", meta = (AllowPrivateAccess = "true"))
	int SpeedIncrease;
};