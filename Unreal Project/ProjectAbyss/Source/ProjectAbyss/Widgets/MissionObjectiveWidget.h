// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actors/MissionSystem/MissionBase.h"
#include "MissionObjectiveWidget.generated.h"

/**
 * 
 */

//class AMissionBase;
class UMissionLogSubsystem;
//struct FMissionDetails;
UCLASS()
class PROJECTABYSS_API UMissionObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	//do this later

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	AMissionBase* MissionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	FMissionDetails MissionDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	UMissionLogSubsystem* MissionLog;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Objective;

	//virtual void NativeConstruct() override;
	//void PreConstruct(bool IsDesignTime) override;


};
