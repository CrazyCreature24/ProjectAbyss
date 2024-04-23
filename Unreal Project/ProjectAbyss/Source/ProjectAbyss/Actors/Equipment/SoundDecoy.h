// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipmentBase.h"
#include "SoundDecoy.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTABYSS_API ASoundDecoy : public AEquipmentBase
{
	GENERATED_BODY()
public:
	
	ASoundDecoy();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void AudioFinished();

	UFUNCTION()
	void Noise();

	virtual void BeginPlay() override;
	virtual void Interacted(ADiverCharacter* Player) override;
	virtual void Tick(float DeltaTime) override;

	virtual void UsePrimary();
	virtual void ReleasePrimary();
	virtual void UseSecondary();
	virtual void ReleaseSecondary();
	virtual void OnDropped();
protected:


public:

	UPROPERTY(EditAnywhere)
	USoundBase* DecoySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* DecoySoundTest;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* DecoyAttenuation;

	FTimerHandle DecoyTimer;
	float TimeElapsed = 0.0f;

	ADiverCharacter* StoredPlayer;

protected:


};
