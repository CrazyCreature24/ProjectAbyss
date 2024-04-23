// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiverController.h"
#include "ControlsUserWidget.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UControlsUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* WASDPanel;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* InteractPanel;

	UPROPERTY(meta = (BindWidget))
		class UBorder* WBorder;

	UPROPERTY(meta = (BindWidget))
		class UBorder* ABorder;

	UPROPERTY(meta = (BindWidget))
		class UBorder* SBorder;

	UPROPERTY(meta = (BindWidget))
		class UBorder* DBorder;

	bool bWBorder = false;
	bool bABorder = false;
	bool bSBorder = false;
	bool bDBorder = false;
public:
	UFUNCTION()
		void PlayerMove();

	UFUNCTION()
		void PlayerInteract();

	FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};
