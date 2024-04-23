// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "Components/BoxComponent.h"

#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "PCTerminal.generated.h"

// make delegate for closing pc

/**
 * 
 */

class UCameraUserWidget;
class UPCUserWidget;
class UMissionUserWidget;
class UShopUserWidget;
class UCollectionJournalWidget;
class UUserManualWidget;

UCLASS()
class PROJECTABYSS_API APCTerminal : public AInteractableActor
{
	GENERATED_BODY()

		APCTerminal();

public:
	virtual void BeginPlay() override;
	
	virtual void Interacted(ADiverCharacter* Player) override;

	void ToggleNotification(bool isNotifOn);

	//Widget Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* PCNotificationWidgetComponent;
	//Light component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPointLightComponent* NotificationLightComponent;

	UPROPERTY()
		UTextureRenderTarget2D* Texture;

	UUserWidget* StoredPCWidget = nullptr;
	UUserWidget* StoredMissionWidget;
	UUserWidget* StoredCameraWidget;
	UUserWidget* StoredShopWidget;
	UUserWidget* StoredCollectionWidget;
	UUserWidget* StoredManualWidget;
	TArray<UUserWidget*> StoredPhotoWidgets;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPCUserWidget> PCWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMissionUserWidget> MissionWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraUserWidget> CameraWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UShopUserWidget> ShopWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCollectionJournalWidget> CollectionWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserManualWidget> ManualWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class UAudioComponent* ComputerSounds;

protected:

	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UWidgetComponent* PCComponent;
};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 14 2023
Name:	Tyler Brost
Summary:	Set up as interactable
-----------------------------

-----------------------------
Date:	 	Oct 18 2023
Name:	Tyler Brost
Summary:	Added journal testing code
-----------------------------

Date:	 	Oct 24 2023
Name:	Tyler Brost
Summary:	Added PC Widget Component
-----------------------------

Date:	 	Feb 16 2024
Name:	Renz Leo Nicolas Dela Cruz
Summary:	Added Audio Component
-----------------------------
*/
