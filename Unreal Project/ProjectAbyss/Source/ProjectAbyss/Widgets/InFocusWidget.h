// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include "InFocusWidget.generated.h"

class ACreature;
/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UInFocusWidget : public UUserWidget
{
	GENERATED_BODY()

	

public:
	/// <summary>
	/// Sets the name of the fish widget to the name of the fish.
	/// </summary>
	/// <param name="Name"></param>
	void SetCreatureName(ACreature* Fish);

	// Construct
	virtual void NativeConstruct() override;

	// Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* name;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* cooldown;

	UPROPERTY(meta = (BindWidget))
		UImage* CameraBox;

	ACreature* StoredCreature;
};
/*
ChangeLog:
-----------------------------
Date:	 	Oct 18 2023
Name:		Austin Morris
Summary:	Focus widget created and ability to change name widget added. 
-----------------------------
-----------------------------
Date:	 	Oct 22 2023
Name:		Austin Morris
Summary:	Dynamic changing of name and image based on captured status added.
-----------------------------
*/
