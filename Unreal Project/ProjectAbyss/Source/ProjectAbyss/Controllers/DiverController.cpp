// Copyright Astralwater Interactive. All Rights Reserved.


#include "DiverController.h"
#include "DiverController.h"
#include "DiverCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "DiverMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractComponent.h"
#include "PhotographyComponent.h"
#include "PhotographyCamera.h"
#include "Components/Border.h"
#include "DiveCage.h"
#include "PCTerminal.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/CameraUserWidget.h"
#include "Ladder.h"
#include "Net/UnrealNetwork.h" //needed for DOREPLIFETIME
#include "Blueprint/UserWidget.h" //needed for widget creation
#include "Components/Widget.h"
#include "../../../Plugins/GameFrwkSessionsPlugin/Source/GameFrwkSessionsPlugin/OnlineGameInstance.h"
#include "InventoryComponent.h"
#include "HotbarWidget.h"
#include "HealthStateComponent.h"
#include "Widgets/HUDWidget.h"
#include <DeployableLight.h>
#include "Divebot.h"
#include "DivebotMovementComponent.h"
#include "Waypoint.h"
#include "CompassWidget.h"
#include "CompassIconWidget.h"
#include "Components/Overlay.h"
#include "WaypointWidget.h"
#include "DivebotCameraOverlayWidget.h"
#include "GameInstance_GameSave/ProjectAbyssGameInstance.h"


ADiverController::ADiverController()
{
	PrimaryActorTick.bCanEverTick = true;
	//Set replication
	bReplicates = true;
	
	
	
}

void ADiverController::Move(const FInputActionValue& Value)
{
	if (myPlayer)
	{
		if (!myPlayer->HealthComponent->GetIsDead())
		{
			Cast<UDiverMovementComponent>(myPlayer->GetMovementComponent())->bIsMoving = true;
			myPlayer->MyMovementComponent->HandleInput(Value);
		}
	}
	else if (myDivebot)
	{
		Cast<UDivebotMovementComponent>(myDivebot->GetMovementComponent())->HandleInput(Value);
	}
}

void ADiverController::StopMove(const FInputActionValue& Value)
{
	if (myPlayer)
	{
		Cast<UDiverMovementComponent>(myPlayer->GetMovementComponent())->bIsMoving = false;
		myPlayer->MyMovementComponent->HandleInput(Value);
	}
	else if (myDivebot)
	{
		Cast<UDivebotMovementComponent>(myDivebot->GetMovementComponent())->HandleInput(Value);
	}
}

void ADiverController::Look(const FInputActionValue& Value)
{

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!myPlayer || !myPlayer->HealthComponent->GetIsDead())
	{
		float PitchChange = 0.0f;
		if (GameInstanceRef->bInvertedControls == false)
		{
			PitchChange = -LookAxisVector.Y * GameInstanceRef->MouseSensitivityY;
		}
		else
		{
			PitchChange = LookAxisVector.Y * GameInstanceRef->MouseSensitivityY;
		}

		// Clamp the pitch to a certain range (e.g., prevent looking all the way down)
		float CurrentPitch = GetControlRotation().Pitch;
		float NewPitch = FMath::ClampAngle(CurrentPitch + PitchChange, -60.0f, 60.0f); // Adjust angles as needed

		// Apply the clamped pitch input
		SetControlRotation(FRotator(NewPitch, GetControlRotation().Yaw, 0.0f));

		// Apply yaw input directly
		AddYawInput(LookAxisVector.X * (GameInstanceRef->MouseSensitivityX * .5));
	}
	if (myDivebot)
	{
		if (GameInstanceRef->bInvertedControls == false)
		{
			AddYawInput(LookAxisVector.X * GameInstanceRef->MouseSensitivityX * .5);
			AddPitchInput(LookAxisVector.Y * GameInstanceRef->MouseSensitivityY * .5);
		}
		else
		{
			AddYawInput(LookAxisVector.X * GameInstanceRef->MouseSensitivityX * .5);
			AddPitchInput(-LookAxisVector.Y * GameInstanceRef->MouseSensitivityY * .5);
		}
	}
}

void ADiverController::ChangeCameraMode(const FInputActionValue& Value)
{
	if (myPlayer)
	{
		if (myPlayer->CameraActor)
		{
			if (myPlayer->CameraActor->GetAimingMode())
			{
				myPlayer->CameraActor->ChangeCameraMode();
			}
		}
	}
}

void ADiverController::Jump()
{
	if(myPlayer && !myPlayer->HealthComponent->GetIsDead())
	{
		Cast<UDiverMovementComponent>(myPlayer->GetMovementComponent())->bIsMoving = true;
		myPlayer->MyMovementComponent->HandleVerticalInput(1);
	}
}

void ADiverController::EndJump()
{
	if (myPlayer)
	{
		Cast<UDiverMovementComponent>(myPlayer->GetMovementComponent())->bIsMoving = false;
		myPlayer->MyMovementComponent->HandleVerticalInput(0);
	}
}

void ADiverController::Crouch()
{
	if (myPlayer && !myPlayer->HealthComponent->GetIsDead())
	{
		Cast<UDiverMovementComponent>(myPlayer->GetMovementComponent())->bIsMoving = true;
		myPlayer->MyMovementComponent->HandleVerticalInput(-1);
	}
}

void ADiverController::StartClimb()
{
	bIsClimbing = true;
}

void ADiverController::StopClimb()
{
	bIsClimbing = false;
}

void ADiverController::RaiseCage()
{
	if (DiveCageRef)	
	{
		//Calls the raise cage function in the dive cage class
		DiveCageRef->RaiseCage();
	}
}

void ADiverController::LowerCage()
{
	if (DiveCageRef)
	{
		//Calls the lower cage function in the dive cage class
		DiveCageRef->LowerCage();
	}
}

void ADiverController::OpenPauseMenu()
{
	if (bRecentlyInMenus == false)
	{
		//Sets input mode to UI only
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = true;

		PauseMenuRef = CreateWidget<UUserWidget>(this, PauseMenuClass);
		PauseMenuRef->AddToViewport();

		//Pause the game
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void ADiverController::Pause()
{
	//Creates widget
	if (PauseMenuClass)
	{
		//Set timer for pause menu
		GetWorldTimerManager().SetTimer(PauseMenuTimerHandle, this, &ADiverController::OpenPauseMenu, 0.001f, false);
	}

}

void ADiverController::Interact(const FInputActionValue& Value)
{
	// Calls interact component interact function

	if (myPlayer)
	{
		// Interacts with an object if has tag interactable
		PlayerInteractDelegate.Broadcast();

		myPlayer->InteractComponent->Interact(myPlayer);
	}
}

void ADiverController::UsePrimary()
{
	// Uses primary of currently held equipment
	if (myPlayer)
	{
		if (!myPlayer->HealthComponent->GetIsDead()) 
		{
			bIsUsingAnything = true;
			if (myPlayer->InventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment))
				{
					myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment)->UsePrimary();
				}
			}
			if (myPlayer->CamInventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10) && bCamSelected == true)
				{
					myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10)->UsePrimary();
				}
			}
		}
		
	}
}

void ADiverController::UseSecondary()
{
	// Stops using secondary of currently held equipment
	if (myPlayer)
	{
		if (!myPlayer->HealthComponent->GetIsDead())
		{
			bIsUsingAnything = true;
			if (myPlayer->InventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment))
				{
					myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment)->UseSecondary();
				}
			}
			if (myPlayer->CamInventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10) && bCamSelected == true)
				{
					myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10)->UseSecondary();
				}
			}
		}
	}
}

void ADiverController::StopUsePrimary()
{
	// Stops using secondary of currently held equipment
	if (myPlayer)
	{
		if (!myPlayer->HealthComponent->GetIsDead())
		{
			bIsUsingAnything = false;
			if (myPlayer->InventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment))
				{
					myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment)->ReleasePrimary();
				}
			}
			if (myPlayer->CamInventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10) && bCamSelected == true)
				{
					myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10)->ReleasePrimary();
				}
			}
		}
		
	}
}

void ADiverController::StopUseSecondary()
{
	// Stops using secondary of currently held equipment
	if (myPlayer)
	{
		if (!myPlayer->HealthComponent->GetIsDead())
		{
			bIsUsingAnything = false;
			if (myPlayer->InventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment))
				{
					myPlayer->InventoryComponent->GetEquipmentAtIndex(HeldEquipment)->ReleaseSecondary();
				}
			}
			if (myPlayer->CamInventoryComponent)
			{
				//Stops using secondary of currently held equipment
				if (myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10) && bCamSelected == true)
				{
					myPlayer->CamInventoryComponent->GetEquipmentAtIndex(10)->ReleaseSecondary();
				}
			}
		}
		
	}
}

void ADiverController::HotbarSlot1()
{
	if (myPlayer)
	{
		if (myPlayer->InventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				bCamSelected = false;
				HotbarWidget->SetSelectedSlot(0);
				HeldEquipment = 0;
				myPlayer->InventoryComponent->SetEquipmentVisiblility(0);
			}
		}
	}
}

void ADiverController::HotbarSlot2()
{
	if (myPlayer)
	{
		if (myPlayer->InventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				bCamSelected = false;
				HotbarWidget->SetSelectedSlot(1);
				HeldEquipment = 1;
				myPlayer->InventoryComponent->SetEquipmentVisiblility(1);
			}
		}
	}
}

void ADiverController::HotbarSlot3()
{
	if (myPlayer)
	{
		if (myPlayer->InventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				bCamSelected = false;
				HotbarWidget->SetSelectedSlot(2);
				HeldEquipment = 2;
				myPlayer->InventoryComponent->SetEquipmentVisiblility(2);
			}
		}
	}
}

void ADiverController::HotbarSlot4()
{
	if (myPlayer)
	{
		if (myPlayer->InventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				bCamSelected = false;
				HotbarWidget->SetSelectedSlot(3);
				HeldEquipment = 3;
				myPlayer->InventoryComponent->SetEquipmentVisiblility(3);
			}
		}
	}
}

void ADiverController::HotbarCamSlot()
{
	if (myPlayer)
	{
		if (myPlayer->CamInventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				bCamSelected = true;
				HotbarWidget->SetSelectedSlot(4);
				HeldEquipment = 10;
				myPlayer->CamInventoryComponent->SetEquipmentVisiblility(0);

				for (int i = 0; i < myPlayer->InventoryComponent->Inventory.Num(); i++)
				{
					if (myPlayer->InventoryComponent->Inventory[i])
					{
						myPlayer->InventoryComponent->Inventory[i]->SetActorHiddenInGame(true);
						myPlayer->InventoryComponent->Inventory[i]->SetIsHeld(false);
					}
				
				}
			}
		}
	}
}

void ADiverController::CycleEquipmentRight()
{
	if (myPlayer)
	{
		if (myPlayer->InventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				HeldEquipment++;

				if (HeldEquipment >= myPlayer->InventoryComponent->Inventory.Num())
				{
					HeldEquipment = 0;
				}

				HotbarWidget->SetSelectedSlot(HeldEquipment);
				myPlayer->InventoryComponent->SetEquipmentVisiblility(HeldEquipment);
			}
		}
	}
}

void ADiverController::CycleEquipmentLeft()
{
	if (myPlayer)
	{
		if (myPlayer->InventoryComponent)
		{
			if (bIsUsingAnything == false)
			{
				HeldEquipment--;

				if (HeldEquipment < 0)
				{
					HeldEquipment = myPlayer->InventoryComponent->Inventory.Num() - 1;
				}

				HotbarWidget->SetSelectedSlot(HeldEquipment);
				myPlayer->InventoryComponent->SetEquipmentVisiblility(HeldEquipment);
			}
		}
	}
}


void ADiverController::BeginPlay()
{
	Super::BeginPlay();

	GameInstanceRef = Cast<UProjectAbyssGameInstance>(GetWorld()->GetGameInstance());

	if (IsLocalPlayerController())
	{
		TSharedRef<FNavigationConfig> Navigation = MakeShared<FNavigationConfig>();
		Navigation->bKeyNavigation = false;
		Navigation->bTabNavigation = false;
		Navigation->bAnalogNavigation = false;
		FSlateApplication::Get().SetNavigationConfig(Navigation);
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALadder::StaticClass(), FoundActors);

	//Make dive cage ref
	if (FoundActors.Num() > 0)
	LadderRef = Cast<ALadder>(FoundActors[0]);

	TArray<AActor*> FoundActors1;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiveCage::StaticClass(), FoundActors1);

	//Make dive cage ref
	if (FoundActors1.Num() > 0)
	DiveCageRef = Cast<ADiveCage>(FoundActors1[0]);

	// Create hotbar widget
	if (IsLocalController())
	{
		if (HotbarWidgetClass)
		{
			HotbarWidget = CreateWidget<UHotbarWidget>(this, HotbarWidgetClass);
			if (HotbarWidget)
				HotbarWidget->AddToViewport();

			if (VideoOverlayWidget)
				HotbarWidget->SetVisibility(ESlateVisibility::Hidden);
			
		}
	}


	// If is local controller
	if (IsLocalController())
	{
		//Debug screen msg
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Local Controller"));
	}

	//if this is the server host
	if (GetLocalRole() == ROLE_Authority)
	{
		//Debug screen msg
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Server Host"));
	}


	HudWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HudWidget)
	{
		HudWidget->AddToViewport();

		if (VideoOverlayWidget)
		{
			HudWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (HeadToBoatWidget)
	{
		HeadToBoatWidget->AddToViewport();

		if (VideoOverlayWidget == nullptr)
			HeadToBoatWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// Creates divebot widget
	VideoOverlayWidget = CreateWidget<UDivebotCameraOverlayWidget>(this, VideoOverlayWidgetClass);
	if (VideoOverlayWidget)
	{
		VideoOverlayWidget->AddToViewport();

		if (myPlayer != nullptr)
			VideoOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	CompassWidget = CreateWidget<UCompassWidget>(this, CompassWidgetClass);
	if (CompassWidget)
	{
		CompassWidget->AddToViewport();
		CompassWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	

}


void ADiverController::SetupInputComponent()
{
		Super::SetupInputComponent();

		//set up inputs
		if (GetLocalRole() == ROLE_Authority || GetLocalRole() == ROLE_AutonomousProxy) // allow server and client to setup the inputs
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
			{
				//Moving
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADiverController::Move);// 1
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ADiverController::StopMove);// 2
				//Jumping
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADiverController::Jump);// 3
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADiverController::EndJump);// 4
				//SwimDown
				EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ADiverController::Crouch);// 5
				EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ADiverController::EndJump);// 6
				//Look
				EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADiverController::Look);// 7
				//Interact
				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADiverController::Interact);// 8
				//Aim camera
				EnhancedInputComponent->BindAction(UseSecondaryAction, ETriggerEvent::Triggered, this, &ADiverController::UseSecondary);// 9

				EnhancedInputComponent->BindAction(UseSecondaryAction, ETriggerEvent::Completed, this, &ADiverController::StopUseSecondary);// 10
				//Take picture
				EnhancedInputComponent->BindAction(UsePrimaryAction, ETriggerEvent::Triggered, this, &ADiverController::UsePrimary);// 11
				EnhancedInputComponent->BindAction(UsePrimaryAction, ETriggerEvent::Completed, this, &ADiverController::StopUsePrimary);// 12

				
				// Raise cage
				EnhancedInputComponent->BindAction(RaiseCageAction, ETriggerEvent::Triggered, this, &ADiverController::RaiseCage);// 14
				// Lower cage
				EnhancedInputComponent->BindAction(LowerCageAction, ETriggerEvent::Triggered, this, &ADiverController::LowerCage);// 15
				// Pause
				EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ADiverController::Pause);// 16
				// Change camera mode
				EnhancedInputComponent->BindAction(ChangeCameraModeAction, ETriggerEvent::Started, this, &ADiverController::ChangeCameraMode);// 17

				//Hotbar
				EnhancedInputComponent->BindAction(HotbarSlot1Action, ETriggerEvent::Started, this, &ADiverController::HotbarSlot1);// 18
				EnhancedInputComponent->BindAction(HotbarSlot2Action, ETriggerEvent::Started, this, &ADiverController::HotbarSlot2);// 19
				EnhancedInputComponent->BindAction(HotbarSlot3Action, ETriggerEvent::Started, this, &ADiverController::HotbarSlot3);// 20
				EnhancedInputComponent->BindAction(HotbarSlot4Action, ETriggerEvent::Started, this, &ADiverController::HotbarSlot4);// 21
				EnhancedInputComponent->BindAction(HotbarCamSlotAction, ETriggerEvent::Started, this, &ADiverController::HotbarCamSlot);

				EnhancedInputComponent->BindAction(CycleEquipmentLeftAction, ETriggerEvent::Started, this, &ADiverController::CycleEquipmentLeft);// 21
				EnhancedInputComponent->BindAction(CycleEquipmentRightAction, ETriggerEvent::Started, this, &ADiverController::CycleEquipmentRight);// 21

				EnhancedInputComponent->BindAction(ReleaseMouseAction, ETriggerEvent::Completed, this, &ADiverController::PlayerMouseReleased);// 23

				//Flashlight
				EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &ADiverController::FlashlightToggle);// 24

				//Tab toggle
				EnhancedInputComponent->BindAction(TabToggleAction, ETriggerEvent::Started, this, &ADiverController::TabToggleOn);// 25
				EnhancedInputComponent->BindAction(TabToggleAction, ETriggerEvent::Completed, this, &ADiverController::TabToggleOff);// 26

				//Scroll hotbar
				EnhancedInputComponent->BindAction(ScrollHotbarAction, ETriggerEvent::Started, this, &ADiverController::ScrollHotbar);// 27

			}
		}
}

void ADiverController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (myPlayer)
	{
		//If the player is climbing, reduce the speed of the player
		if (bIsClimbing)
		{
			myPlayer->GetCharacterMovement()->MaxWalkSpeed = 25;
		}
		else
		{
			// If the player is not climbing, set the speed to the default speed
			myPlayer->GetCharacterMovement()->MaxWalkSpeed = 600;
		}

	}
}

void ADiverController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADiverController, myPlayer);
}


void ADiverController::FlashlightToggle()
{
	if (myPlayer)
	{
		if (myPlayer->FlashlightComponent->Intensity == 0)
		{
			myPlayer->FlashlightComponent->SetIntensity(15000);
			HudWidget->ToggleFlashlight(true);
		}
		else
		{
			myPlayer->FlashlightComponent->SetIntensity(0);
			HudWidget->ToggleFlashlight(false);
		}
	}
}

void ADiverController::TabToggleOn()
{
	//if (HudWidget)
	//{
	//	HudWidget->SetVisibility(ESlateVisibility::Visible);
	//}

	// If the compass widget exists, set its visibility to true
	if (CompassWidget)
{
		CompassWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (HudWidget)
	{
		HudWidget->ToggleWaypointVisibility(true);
	}

	//Gets all waypoint actors in the scene and sets their visibility to true
	TArray<AActor*> Waypoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), Waypoints);
	if (Waypoints.Num() > 0)
	{
		for (AActor* Waypoint : Waypoints)
		{
			if (GetPawn()->ActorHasTag("Boat"))
			{
				//If the waypoint has the type of boat, set its visibility to false
				if (Cast<AWaypoint>(Waypoint)->WaypointType == EWaypointType::WT_Boat)
				{
					Waypoint->SetActorHiddenInGame(true);
				}
				else
				{
					Waypoint->SetActorHiddenInGame(false);
				}
			}
			else
			{
				Waypoint->SetActorHiddenInGame(false);
			}
		}
	}

	if (GetPawn()->ActorHasTag("Boat"))
	{
		TArray<AActor*> temparray;

		//Get PC terminal actor from world
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);

		APCTerminal* tempPC = Cast<APCTerminal>(temparray[0]);

		tempPC->StoredCameraWidget = CreateWidget(Cast<APlayerController>(this), tempPC->CameraWidget);
		tempPC->StoredCameraWidget->AddToViewport();

		//Display the map widget and set focus to it
		//MapHotbar->SetVisibility(ESlateVisibility::Visible);
		//Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetFocus();
		//Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetKeyboardFocus();

		Cast<UWidget>(Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->MoveBorderCamera)->SetVisibility(ESlateVisibility::Collapsed);
		Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->MapSlider->SetVisibility(ESlateVisibility::Collapsed);

		Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->SetRenderOpacity(0.5f);
	}

}

void ADiverController::TabToggleOff()
{
	//if (HudWidget)
	//{
	//	HudWidget->SetVisibility(ESlateVisibility::Hidden);
	//}

	// If the compass widget exists, set its visibility to false
	if (CompassWidget)
	{
		CompassWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (HudWidget)
	{
		HudWidget->ToggleWaypointVisibility(false);
	}

	//Gets all waypoint actors in the scene and sets their visibility to true
	TArray<AActor*> Waypoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), Waypoints);
	if (Waypoints.Num() > 0)
	{
		for (AActor* Waypoint : Waypoints)
		{
			Waypoint->SetActorHiddenInGame(true);
		}
	}

	if (GetPawn()->ActorHasTag("Boat"))
	{
		TArray<AActor*> temparray;

		//Get PC terminal actor from world
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);

		APCTerminal* tempPC = Cast<APCTerminal>(temparray[0]);

		Cast<UWidget>(Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->MoveBorderCamera)->SetVisibility(ESlateVisibility::Visible);
		Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->MapSlider->SetVisibility(ESlateVisibility::Visible);
		Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->SetRenderOpacity(1.0f);

		tempPC->StoredCameraWidget->RemoveFromParent();
		tempPC->StoredCameraWidget = nullptr;

		//Display the map widget and set focus to it
		//MapHotbar->SetVisibility(ESlateVisibility::Visible);
		//Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetFocus();
		//Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetKeyboardFocus();


	}
}

void ADiverController::ScrollHotbar(const FInputActionValue& Value)
{
	if (HeldEquipment != 10)
	{
		if (Value.GetMagnitude() > 0.0f)
		{
			//Scroll up
			CycleEquipmentRight();
		}
		else if (Value.GetMagnitude() < 0.0f)
		{
			//Scroll down
			CycleEquipmentLeft();
		}
	}
}

void ADiverController::SetRecentlyInMenusFalse()
{
	bRecentlyInMenus = false;
}

void ADiverController::StartRecentlyInMenusTimer()
{
	bRecentlyInMenus = true;
	GetWorldTimerManager().SetTimer(RecentlyInMenusTimerHandle, this, &ADiverController::SetRecentlyInMenusFalse, 0.2f, false);
}


void ADiverController::PlayerMouseReleased()
{
	PlayerMouseUpDelegate.Broadcast();
}

void ADiverController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	{

		myPlayer = Cast<ADiverCharacter>(aPawn);
		if (myPlayer)
		{
			myPlayer->Controller = this;
			if (HudWidget && HotbarWidget)
			{
				HudWidget->SetVisibility(ESlateVisibility::Visible);
				HotbarWidget->SetVisibility(ESlateVisibility::Visible);
			}

			if (VideoOverlayWidget)
			{
				VideoOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			return;
		}

		myDivebot = Cast<ADivebot>(aPawn);

		if (myDivebot)
		{
			myDivebot->Controller = this;
			return;
		}
	}
}

void ADiverController::OnUnPossess()
{
	Super::OnUnPossess();

	if (Owner != myPlayer)
	{
		if (HudWidget)
		{
			HudWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		if (HotbarWidget)
		{
			HotbarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (Owner == myPlayer)
	{
			TArray<AActor*> temparray;

			//Get PC terminal actor from world
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);

			if (GetWorld()->GetMapName() == L"UEDPIE_0_OverWorld")
			{
				APCTerminal* tempPC = Cast<APCTerminal>(temparray[0]);

				if (tempPC->StoredCameraWidget != nullptr)
				{
					Cast<UWidget>(Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->MoveBorderCamera)->SetVisibility(ESlateVisibility::Visible);
					Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->MapSlider->SetVisibility(ESlateVisibility::Visible);
					Cast<UCameraUserWidget>(tempPC->StoredCameraWidget)->SetRenderOpacity(1.0f);

					tempPC->StoredCameraWidget->RemoveFromParent();
					tempPC->StoredCameraWidget = nullptr;
				}
			}
	}
}

/*
ChangeLog:
-----------------------------
Date:	 	Oct 12 2023
Name:		Maxime Paradis
Summary:	Created controller to be used by player for movement
-----------------------------
Date:	 	Oct 13 2023
Name:		Maxime Paradis
Summary:	Created functions and licked the to player allowing movement
-----------------------------
Date:	 	Oct 19 2023
Name:		Maxime Paradis
Summary:	Made change for future networking and comented relevent code
-----------------------------
-----------------------------
Date:	 	Nov 1 2023
Name:		Austin Morris
Summary:	Added zoom with mouse wheel input instead of QE
-----------------------------
*/