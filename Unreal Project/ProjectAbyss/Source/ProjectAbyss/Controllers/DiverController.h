// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once
/// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "DiverCharacter.h"
#include "../GameStates/MainGameState.h"
#include "HotbarWidget.h"
#include "CompassIconWidget.h"
#include "DiverController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerMouseUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerMove);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerInteract);
/**
 *
 */
UCLASS()
class PROJECTABYSS_API ADiverController : public APlayerController
{
	GENERATED_BODY()

public:

	//Input Properties----------------------------------------

	FPlayerMouseUp PlayerMouseUpDelegate;
	FPlayerMove PlayerMoveDelegate;
	FPlayerInteract PlayerInteractDelegate;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Release Mouse Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReleaseMouseAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Interact Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Lower Cage Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LowerCageAction;

	/** Raise Cage Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RaiseCageAction;

	/** Aim Camera Action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UseSecondaryAction;

	/** Take Photo Action **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UsePrimaryAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Pause Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;

	/** Hotbar Slot 1 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HotbarSlot1Action;

	/** Hotbar Slot 2 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HotbarSlot2Action;

	/** Hotbar Slot 3 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HotbarSlot3Action;

	/** Hotbar Slot 4 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HotbarSlot4Action;

	/** Hotbar Cam Slot Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HotbarCamSlotAction;

	/** Cycle Equipment Left Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CycleEquipmentLeftAction;

	/** Cycle Equipment Right Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CycleEquipmentRightAction;

	/** Change Camera Mode Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChangeCameraModeAction;

	/** Flashlight Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FlashlightAction;

	/** Tab hud toggle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* TabToggleAction;

	/** Scroll mouse equipment swap Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ScrollHotbarAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"), Replicated)
	ADiverCharacter* myPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"), Replicated)
	ADivebot* myDivebot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HotbarWidgetClass;

	UHotbarWidget* HotbarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CompassWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> VideoOverlayWidgetClass;

	class UDivebotCameraOverlayWidget* VideoOverlayWidget;

	class UHUDWidget* HudWidget;

	class UCompassWidget* CompassWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pop Ups", meta = (AllowPrivateAccess = "true"))
	UUserWidget* HeadToBoatWidget;

	TArray<UCompassIconWidget*> CompassIconWidgets;

	//Timer handle for pause menu
	FTimerHandle PauseMenuTimerHandle;

	void OpenPauseMenu();

	//Input Methods----------------------------------------

	ADiverController();

	/// <summary>
	/// calls the players handle inputs
	/// </summary>
	/// <param name="Value">Value given by the input component</param>
	void Move(const FInputActionValue& Value);

	/// <summary>
	/// calls the players handle input
	/// </summary>
	/// <param name="Value"Value given by the input component></param>
	void StopMove(const FInputActionValue& Value);

	/// <summary>
	/// Sets the controllers rotation
	/// </summary>
	/// <param name="Value">2d vector given by input component</param>
	void Look(const FInputActionValue& Value);


	// Change camera mode
	void ChangeCameraMode(const FInputActionValue& Value);

	/// <summary>
	/// Calls acharater jump
	/// </summary>
	void Jump();

	/// <summary>
	/// Calls acharacter endjunmp
	/// </summary>
	void EndJump();

	/// <summary>
	/// Calls the swim down 
	/// </summary>
	void Crouch();

	// Climbs the ladder
	void StartClimb();

	// Stops climbing the ladder
	void StopClimb();

	// Raises the cage
	void RaiseCage();

	// Lowers the cage
	void LowerCage();

	// Pauses the menu
	void Pause();

	/// <summary>
	/// causes player in interact
	/// </summary>
	/// <param name="Value"></param>
	void Interact(const FInputActionValue& Value);


	// Use primary
	void UsePrimary();

	// Use secondary
	void UseSecondary();

	// Stop using primary
	void StopUsePrimary();

	// Stop using secondary
	void StopUseSecondary();

	// Hotbar slot 1
	void HotbarSlot1();

	// Hotbar slot 2
	void HotbarSlot2();

	// Hotbar slot 3
	void HotbarSlot3();

	// Hotbar slot 4
	void HotbarSlot4();

	// Hotbar slot 4
	void HotbarCamSlot();

	/// <summary>
	/// When pressed, this method increments the equipped piece of equipment, and changes the displays the change on the hotbar. This is used for Controller support.
	/// </summary>
	void CycleEquipmentRight();

	/// <summary>
	/// When pressed, this method decrements the equipped piece of equipment, and changes the displays the change on the hotbar. This is used for Controller support.
	/// </summary>
	void CycleEquipmentLeft();

	/// <summary>
	/// When pressed, this method toggles the flashlight on and off. This is used for Controller support.
	/// </summary>
	void FlashlightToggle();

	/// <summary>
	/// When pressed, this method toggles the HUD on and off. Waypoints too.
	/// </summary>
	void TabToggleOn();

	/// <summary>
	/// When released, this method toggles the HUD on and off. Waypoints too.
	/// </summary>
	void TabToggleOff();

	/// <summary>
	/// When pressed, this method scrolls the hotbar up and down depending on input.
	/// </summary>
	void ScrollHotbar(const FInputActionValue& Value);

	/// <summary>
	/// Gets pause menu reference
	/// </summary>
	/// <returns>Pause menu reference</returns>
	FORCEINLINE TSubclassOf<UUserWidget> GetPauseMenu() const { return PauseMenuClass; }


	// Called when player releases Mouse 1
	void PlayerMouseReleased();

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	class UProjectAbyssGameInstance* GameInstanceRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "0.2", ClampMax = "2.0", UIMin = "0.2", UIMax = "2.0"))
	float SoundVolume = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bIsWaypointsAlwaysOn = false;

	bool bRecentlyInMenus = false;

	//Set recently in menu
	void SetRecentlyInMenusFalse();

	//Timer handle for recently in menus
	FTimerHandle RecentlyInMenusTimerHandle;

	//Start recently in menus timer
	void StartRecentlyInMenusTimer();


	// Currently held equipment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Variables")
	int32 HeldEquipment = 0;

	bool bIsClimbing = false;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	class ALadder* LadderRef;
	class ADiveCage* DiveCageRef;
	UUserWidget* PauseMenuRef;

	// If the player is holding right or left click.
	bool bIsUsingAnything = false;

	bool bCamSelected = true;

};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 10 2023
Name:		Maxime Paradis
Summary:	Created and populated with basic implementation
-----------------------------
Date:	 	Oct 17 2023
Name:		Alexander McTernan
Summary:	Tried to add some code to swap mapping contexts. Not working yet. Commented out.
-----------------------------
-----------------------------
Date:	 	Nov 14 2023
Name:		Austin Morris
Summary:	Added inventory component to player and added functions to use items based on held equipment.
-----------------------------
Date:	 	Dec 2 2023
Name:		Alexander McTernan
Summary:	Added new ZoomIn/Out methods, as well as Cycle equipment methods to implement controller support. Also added input actions for these as well.
			Added bInvertedCoontrols so we can invert the Y axis on look on the player.
-----------------------------
Date:	 	Dec 3 2023
Name:		Alexander McTernan
Summary:	Cleaned up the Zoom methods.
			Added mouse sensitivity. MouseSensitivityX and MouseSensitivityY
-----------------------------
*/