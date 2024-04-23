// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "IndividualPhotoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UIndividualPhotoWidget : public UBaseUserWidget
{
	GENERATED_BODY()

	public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetupPhoto(UMaterialInstanceDynamic* Photo);
	
	//Button handling
	UFUNCTION()
		void CloseButtonClicked();

	//Buttons
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;


	UFUNCTION()
		void MoveWidget();
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//Border for moving widget
	UPROPERTY(meta = (BindWidget))
		class UBorder* MoveBorder;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* MovableOverlay;

	//Box containing all other elements
	UPROPERTY(meta = (BindWidget))
		class UScaleBox* ContainingScaleBox;

	bool bMoveWidget = false;
	TArray<std::reference_wrapper<bool>> Bools{ bMoveWidget };

	double PreviousMouseX;
	double PreviousMouseY;

	//References
	class APCTerminal* PC;
	class AStorageChest* StorageChest;

	// Bind image widget
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* StoredPhoto;
};
