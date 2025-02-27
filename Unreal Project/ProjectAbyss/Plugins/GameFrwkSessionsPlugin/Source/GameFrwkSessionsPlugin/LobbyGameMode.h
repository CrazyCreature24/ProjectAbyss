// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFrwkSessionsPlugin.h"
#include "CoreMinimal.h"
#include "OnlineGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFRWKSESSIONSPLUGIN_API ALobbyGameMode : public AOnlineGameMode
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

    UPROPERTY(EditAnywhere, Category = "Max Players")
        uint32 MaxNumberOfPlayers = 1;


private:

	void StartGame();

	uint32 NumberOfPlayers = 0;

	FTimerHandle GameStartTimer;
};
