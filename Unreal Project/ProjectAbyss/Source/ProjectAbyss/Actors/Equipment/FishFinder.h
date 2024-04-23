// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipmentBase.h"
#include "FishFinder.generated.h"

UCLASS()
class PROJECTABYSS_API AFishFinder : public AEquipmentBase
{
	GENERATED_BODY()

public:
	AFishFinder();

    //Box components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UBoxComponent* Hitbox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UWidgetComponent* FishFinderWidget;


    //Equip override
    virtual void Equip(AActor* NewOwner) override;

protected:
	virtual void UsePrimary() override;
	virtual void ReleasePrimary() override;

	virtual void UseSecondary() override;
	virtual void ReleaseSecondary() override;

    //BeginPlay
    virtual void BeginPlay() override;



    UFUNCTION()
        void EnableCollision();

    UFUNCTION()
        void ScaleHitbox();

    UFUNCTION()
        void OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
            const FHitResult& SweepResult);

    TSubclassOf<class AFishFinderWidget> FishFinderWidgetClass;

    // Array for scanned fish
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ACreature*> ScannedFish;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float GrowthRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector MaxSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector CurrentSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float InitialDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float ScaleInterval;

    FTimerHandle EnableCollisionTimerHandle;
    FTimerHandle ScaleTimerHandle;

    int FishCount = 0;
};
