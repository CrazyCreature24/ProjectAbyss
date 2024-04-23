// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PhotographyCollectionSubsystem.generated.h"

class ACreature;



/**
 * 
 */
UCLASS(BlueprintType, Abstract, meta = (BlueprintSpawnableComponent))
class PROJECTABYSS_API UPhotographyCollectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPhotographyCollectionSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:


	
};
