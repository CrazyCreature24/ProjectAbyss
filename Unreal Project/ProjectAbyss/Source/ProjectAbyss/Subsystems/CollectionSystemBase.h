// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhotographyCollectionSubsystem.h"
#include "GameFramework/Actor.h"
#include "Creature.h"
#include "CollectionSystemBase.generated.h"


USTRUCT(BlueprintType)
struct FCreatureEntryWrapper
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACreature> Creature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFirstTimeBonusAvailable = true;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int CollectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreatureName;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UTexture2D* CreatureIconImage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//ECreatureType CreatureType;
};

UCLASS()
class PROJECTABYSS_API ACollectionSystemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectionSystemBase();

	// CHANGES MADE TO THIS VARIABLE IS WHAT IS SAVED TO THE SAVE GAME OBJECT FOR THE COLLECTION SYSTEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography Collection")
	TArray<FCreatureEntryWrapper> CreatureCollectionArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Save the collection data to the save game object
	UFUNCTION(BlueprintCallable, Category = "Photography Collection")
		void SaveCollectionData();

	// Load the collection data from the save game object
	UFUNCTION(BlueprintCallable, Category = "Photography Collection")
		void LoadCollectionData();

	UFUNCTION(BlueprintCallable, Category = "Photography Collection")
		void UpdateInfoOfFishInWorld(int FishIndex, FString NewName);

		virtual void OnEndPlay();

};
