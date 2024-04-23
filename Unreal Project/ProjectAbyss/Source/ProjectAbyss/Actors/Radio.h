// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "Radio.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API ARadio : public AInteractableActor
{
	GENERATED_BODY()
	
public:

	ARadio();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Sound")
	class UMediaSoundComponent* SoundComp;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
