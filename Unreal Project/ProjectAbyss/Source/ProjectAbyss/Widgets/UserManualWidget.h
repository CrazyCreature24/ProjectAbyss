#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "UserManualWidget.generated.h"

class UButton;
class UOverlay;
class UBorder;
class UWidgetSwitcher;
class UTextBlock;

UCLASS()
class PROJECTABYSS_API UUserManualWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UUserManualWidget();

	UFUNCTION()
		void MoveWidget();
	UFUNCTION()
		void Close();
	UFUNCTION()
		void OnHotbarButtonClicked();
	UFUNCTION()
		void MinimizeButtonClicked();

	UFUNCTION()
		void RightPage1ButtonClicked();
	UFUNCTION()
		void RightPage2ButtonClicked();
	UFUNCTION()
		void RightPage3ButtonClicked();
	UFUNCTION()
		void RightPage4ButtonClicked();
	UFUNCTION()
		void RightPage5ButtonClicked();
	UFUNCTION()
		void RightPage6ButtonClicked();
	UFUNCTION()
		void RightPage7ButtonClicked();
	UFUNCTION()
		void RightPage8ButtonClicked();


	UFUNCTION()
		void LeftPage2ButtonClicked();
	UFUNCTION()
		void LeftPage3ButtonClicked();
	UFUNCTION()
		void LeftPage4ButtonClicked();
	UFUNCTION()
		void LeftPage5ButtonClicked();
	UFUNCTION()
		void LeftPage6ButtonClicked();
	UFUNCTION()
		void LeftPage7ButtonClicked();
	UFUNCTION()
		void LeftPage8ButtonClicked();
	UFUNCTION()
		void LeftPage9ButtonClicked();

protected:


	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;



public:

	bool bMoveWidget = false;
	UPROPERTY(meta = (BindWidget))
		UOverlay* MovableOverlay;

	TArray<std::reference_wrapper<bool>> Bools{ bMoveWidget };

protected:

	double PreviousMouseX;
	double PreviousMouseY;

	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		UButton* MinimizeButton;
	//Animation for minimizing widget
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* MinimizeAnimation;

	UPROPERTY(meta = (BindWidget))
		UBorder* MoveBorder;


	//TABS
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		UButton* RightPage1_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage2_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage3_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage4_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage5_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage6_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage7_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* RightPage8_Button;






	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage2_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage3_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage4_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage5_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage6_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage7_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage8_Button;
	UPROPERTY(meta = (BindWidget))
		UButton* LeftPage9_Button;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundBase* PageFlipSound;


	APCTerminal* PC;

};