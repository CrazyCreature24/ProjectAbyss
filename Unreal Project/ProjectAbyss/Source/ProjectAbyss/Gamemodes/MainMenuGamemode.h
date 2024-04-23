// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGamemode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API AMainMenuGamemode : public AGameModeBase
{
	GENERATED_BODY()

	public:

		AMainMenuGamemode();

		virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

		/// <summary>
		/// Starts to move the cage down when player presses the screen. !!!(THIS IS CALLED IN WIDGETS)!!!
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "Main Menu")
		void MoveCageDown();

		/// <summary>
		/// Stops the cage from moving down. 
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "Main Menu")
		void StopCageDown();

		/// <summary>
		/// Starts to move the fish right when the cage is at the bottom.
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "Main Menu")
			void MoveFishRight();

		/// <summary>
		/// Stops the fish from moving right when its at the farthest point. (In front of the camera)
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "Main Menu")
			void StopMoveFishRight();


		// Moves cage up to hide the main menu
		
		// Lowest point the cage can go
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
		float CageLowestPoint = -8264.0f;

		// Farthest point the fish can go to the right
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
		float FishFarthestPoint = 4092.0f;

		// How fast the cage moves
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
		float CageMoveSpeed = 500.0f;

		// How fast the fish moves
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
		float FishMoveSpeed = 300.0f;

	    // Dive cage ref
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
		class ADiveCage* CageRef;

		// Main menu ref
		UPROPERTY(EditDefaultsOnly, Category = "Main Menu")
		TSubclassOf<UUserWidget> MainMenuWidgetClass;

		// Fish eye ref 
		UPROPERTY(EditDefaultsOnly, Category = "Main Menu")	
		class ABigMenuEyeball* FishEyeRef;

		int test = 0;



	bool bIsCageMoving = false;
	bool bIsFishMoving = false;
};

/*
ChangeLog:
-----------------------------
Date:	 	Nov 11 2023
Name:		Austin Morris
Summary:	Created base class and added basic functionality for main menu. Changed TBD for the eyeball, animations, and the main menu widget.
-----------------------------

*/
