// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
	AMainGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Adds player to list when they join the game.
	/// </summary>
	/// <param name="NewPlayer"></param>
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 Money = 100;

	FORCEINLINE int32 GetMoney() { return Money; }
	FORCEINLINE void SetMoney(int32 moneys) { Money = moneys; }
	FORCEINLINE void AddMoney(int32 moneystoadd) { Money += moneystoadd; }
	FORCEINLINE void SubtractMoney(int32 moneystosubtract) { Money -= moneystosubtract; }

	// List of PlayerControllers
	UPROPERTY()
	TArray<APlayerController*> PlayerControllerList;
};

