// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"


class ABoat;
class ADivebot;
class ADiverCharacter;
class AWaterTransitionVolume;

UCLASS()
class PROJECTABYSS_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

	public:
	AMainGameState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	inline ABoat*					GetBoat()			{ return GameBoat; }
	inline ADiverCharacter*			GetPlayer()			{ return GameCharacter; }
	inline ADivebot*				GetBot()			{ return GameDiveBot; }
	inline AWaterTransitionVolume*	GetWaterVolume()	{ return GameWaterVolume; }

	inline void SetBoat(ABoat* b)							{ GameBoat = b; }
	inline void SetPlayer(ADiverCharacter* c)				{ GameCharacter = c; }
	inline void SetBot(ADivebot* b)							{ GameDiveBot = b; }
	inline void SetWaterVolume(AWaterTransitionVolume* v)	{ GameWaterVolume = v; }

	void SpawnDivebot();
	void SpawnDivebotNearDiver();
	void OnLevelTransferSpawnAsDivebot();
	void DeSpawnDivebot();

	UPROPERTY(EditAnywhere, Category = "Divebot")
	TSubclassOf<AActor> DiveBotClass;

	void PlayerDied();// { bPlayerHasDied = true; }
	inline bool GetPlayerHasDied() { return bPlayerHasDied; }
	void ResetPlayerState();

	/// <summary>
	/// Sets the player position to the respawn point onboard of the ship
	/// Function called on to respawn player
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerDeath")
	void RespawnPlayer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Maps")
	void LoadOverWorld();

protected:
	ABoat* GameBoat;
	ADiverCharacter* GameCharacter;
	ADivebot* GameDiveBot;
	AWaterTransitionVolume* GameWaterVolume;

	bool bPlayerHasDied = false;
	bool bFirstTick = true;
};

/*
ChangeLog:
-----------------------------
Date:	 	Nov 11 2023
Name:		Austin Morris
Summary:	Created base class for future game state functionality. Added player role enum. Added functions to set and get player role.
-----------------------------
Date:	 	Feb 4 2024
Name:		Maxime Paradis
Summary:	Romved enum, added important game getters for player, divebot and boat as well as respawning helper functions.
-----------------------------
Date:	 	Feb 12
Name:		Maxime Paradis
Summary:	Added WaterVolume getter setters. 
-----------------------------
Date:	 	April 4th
Name:		Alexander McTernan
Summary:	Swap player mesh on respawn.
-----------------------------
*/

