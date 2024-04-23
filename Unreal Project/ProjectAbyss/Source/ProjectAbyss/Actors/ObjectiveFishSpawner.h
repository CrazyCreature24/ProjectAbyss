// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveFishSpawner.generated.h"

UCLASS()
class PROJECTABYSS_API AObjectiveFishSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveFishSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void SpawnFish();

	UFUNCTION(BlueprintCallable)
	void GetObjectiveIDsFromMissions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishesThatSpawn")
	TArray<TSubclassOf<class ACreature>> CreaturesInMap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectiveIDs")
	TArray<FString> ActiveMissionObjectiveIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnLocations")
	TArray<AActor*> SpawnLocations;

};
