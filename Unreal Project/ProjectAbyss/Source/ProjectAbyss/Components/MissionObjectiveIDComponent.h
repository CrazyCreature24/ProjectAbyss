// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MissionObjectiveIDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UMissionObjectiveIDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMissionObjectiveIDComponent();

protected:
	// Called when the game starts (not used)
	virtual void BeginPlay() override;

public:	
	// Called every frame (Not used)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Variable")
	TArray<FString> ObjectiveIDs;

public:

	/// <summary>
	/// For each objective ID in the array, call CallObjectiveID in mission log with that given ID.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void BroadcastObjectiveIDs(UObject* WorldContextObject);
};

/*
ChangeLog:
-----------------------------
Date:		Oct 30 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	Component Creation
-----------------------------
Date:		Nov 13 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Renaming "Quest" to "Objective"
-----------------------------
Date:		Jan 19 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Adding comments
-----------------------------
*/