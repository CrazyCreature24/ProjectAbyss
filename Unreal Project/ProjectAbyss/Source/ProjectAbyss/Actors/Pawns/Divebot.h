// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatetransitionInterface.h"
#include "Divebot.generated.h"

UCLASS()
class PROJECTABYSS_API ADivebot : public ACharacter, public IStateTransitionInterface
{
	GENERATED_BODY()

public:
	ADivebot(const FObjectInitializer& ObjectInitializer);
	~ADivebot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DESIGNERS_Specs|Setup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	// ------------------ Controls ------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DESIGNERS_Specs|Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DivebotMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DESIGNERS_Specs|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DESIGNERS_Specs|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DESIGNERS_Specs|Setup", meta = (AllowPrivateAccess = "true"))
	float BatteryLifeInSeconds = 120;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DESIGNERS_Specs|Setup", meta = (AllowPrivateAccess = "true"))
	float GracePeriodTime = 10;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Handles the transition from walking to swimming
	/// </summary>
	void TransitionMovementStates() override;

	void SetToIsSwimming() override;

	///<summary> When the divebot is not possessed, this function is called. This calls the method of the same name on the MovementComponent </summary>
	void FollowDiver(float DeltaTime);

	void PossessDivebot();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	//Flashlight on players head
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight", meta = (AllowPrivateAccess = "true"))
	class USpotLightComponent* FlashlightComponent;

	class UDivebotMovementComponent* MovementComponent;

	class UUserWidget* StoredOverlayWidget;

	FTimerHandle popUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pop Ups", meta = (AllowPrivateAccess = "true"))
	float PopUpLifeTime = 10;

	ACharacter* player = nullptr;

	void GetToShip(USceneComponent* spawnPoint);
	void CancelGetToShip();
	void DivebotDied();
	void TurnOffPopUp();

	bool bIsPossessed;

	FTimerHandle Grace;

private:
	bool bGoToShip = false;

	bool bIsSwiming = true;

	float BateryLifeOG;
	int NextBatteryThreshold = 75;

};



/*
ChangeLog:
-----------------------------
Date:	 	Oct 14 2023
Name:		Alexander McTernan
Summary:	Initial creation. Ability for the bot to follow the divebot the diver over the shoulder.
-----------------------------
Date:	 	Oct 15 2023
Name:		Alexander McTernan
Summary:	Possession created and movement is now possible. Unpossess is a problem.
-----------------------------
Date:	 	Oct 16 2023
Name:		Alexander McTernan
Summary:	Unpossess is a problem. Creating divebot controller to do the input.
-----------------------------
Date:	 	Oct 17 2023
Name:		Alexander McTernan
Summary:	Moved all the input into 1 mapping context. Divebot controller is not connected but commented code show my progress on both of these things.
			Player is now able to possess and unpossess the divebot flawlessly after moving to 1 mapping context.
			Below are a few links that I was looking into to solve my problems.
			https://forums.unrealengine.com/t/how-to-switch-to-another-player-controller-with-c/303687/2
			https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/GameFramework/AGameModeBase/SwapPlayerControllers/
			https://forums.unrealengine.com/t/how-to-change-between-multiple-input-mapping-contexts/1146354
			Commented everything!
-----------------------------
Date:	 	Oct 30 2023
Name:		Alexander McTernan
Summary:	Added Photography Functionality. All works except the pictures are not available on the PC yet, but that is done somewhere else.
			Created a separate movement Component to handle all of the math for that. It handles the exact logic the divebot did before. May adjust later.
-----------------------------
Date:	 	Oct 31 2023
Name:		Alexander McTernan
Summary:	Adding Networking support. Replicated variables and set the actor and its movement to be replicated.
			Also added some includes that were missing to fix some intellisense errors. There were no build issues before, but unreal wanted them anyway.
			Added a ServerPossess function in an effort to fix the client possession problem. https://forums.unrealengine.com/t/possess-on-client-not-working-properly/397042/4
-----------------------------
Date:	 	Nov 2nd 2023
Name:		Alexander McTernan
Summary:	Found out how to Possess the divebot on the Client. Set up a NetMulticast Possess method that calls the regular DivebotPossess, and calling this one when the client wants to possess.
			Replicated more variables to try to  make the clients movement replicate to the server. Not successful.
			Also made it so only one player can control the Divebot at a time.
-----------------------------
Date:	 	Nov 5th 2023
Name:		Alexander McTernan
Summary:	More work in attempting to replicate the clients movement to the server by switching the Controlled movement to be a NetMulticast function. Didn't work.
			Then tried to have it be a Server function and then have it NetMulticast down to the clients with the UpdatePosition method. Didn't work.
			Tried a few other variations on this method with no success.
-----------------------------
Date:	 	Nov 7th 2023
Name:		Alexander McTernan
Summary:	Cleaned up code for Release 1. Made a couple Ignore blocks of code to keep for networking debugging. Added a couple missing method summaries.
-----------------------------
Date:	 	Dec 3 2023
Name:		Alexander McTernan
Summary:	Added mouse sensitivity. MouseSensitivityX and MouseSensitivityY.
			Accounted for inverted look controls as well.
-----------------------------
Date:	 	Feb 5 2024
Name:		Maxime Paradis
Summary:	Remade it all.
-----------------------------
*/

