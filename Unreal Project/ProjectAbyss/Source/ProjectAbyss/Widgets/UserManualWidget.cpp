// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/UserManualWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UUserManualWidget::UUserManualWidget()
{
}

void UUserManualWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PC = GetPC();

	MoveBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("MoveWidget"));

	RightPage1_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage1ButtonClicked);
	RightPage2_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage2ButtonClicked);
	RightPage3_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage3ButtonClicked);
	RightPage4_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage4ButtonClicked);
	RightPage5_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage5ButtonClicked);
	RightPage6_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage6ButtonClicked);
	RightPage7_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage7ButtonClicked);
	RightPage8_Button->OnClicked.AddDynamic(this, &UUserManualWidget::RightPage8ButtonClicked);

	LeftPage2_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage2ButtonClicked);
	LeftPage3_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage3ButtonClicked);
	LeftPage4_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage4ButtonClicked);
	LeftPage5_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage5ButtonClicked);
	LeftPage6_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage6ButtonClicked);
	LeftPage7_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage7ButtonClicked);
	LeftPage8_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage8ButtonClicked);
	LeftPage9_Button->OnClicked.AddDynamic(this, &UUserManualWidget::LeftPage9ButtonClicked);



	WidgetSwitcher->SetActiveWidgetIndex(0);
	//0 = Page 1
	//1 = Page 2
	//2 = Page 3

	//Bind buttons
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UUserManualWidget::Close);
	}
	if (MinimizeButton)
	{
		MinimizeButton->OnClicked.AddDynamic(this, &UUserManualWidget::MinimizeButtonClicked);
	}

}

void UUserManualWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bMoveWidget == true)
	{
		UpdateBorder(this, Cast<UCanvasPanelSlot>(MovableOverlay->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	}

	FVector2D mouse;
	//Get mouse position after scaling/moving
	mouse = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	PreviousMouseX = mouse.X;
	PreviousMouseY = mouse.Y;
}

FReply UUserManualWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UnsetBools();

	return FReply::Handled();
}

void UUserManualWidget::OnHotbarButtonClicked()
{
	//Displays the widget again and hides the image
	Cast<UPCUserWidget>(PC->StoredPCWidget)->ManualIsMinimized->SetVisibility(ESlateVisibility::Hidden);
	PlayAnimation(MinimizeAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UUserManualWidget::Close()
{
	Cast<UPCUserWidget>(PC->StoredPCWidget)->ManualHotbar->SetVisibility(ESlateVisibility::Hidden);
	RemoveFromParent();

	//Sets widget to null for garbage collection
	PC->StoredManualWidget = nullptr;
}

void UUserManualWidget::MinimizeButtonClicked()
{
	//Play animation to hide widget
	PlayAnimation(MinimizeAnimation);
	Cast<UPCUserWidget>(PC->StoredPCWidget)->ManualIsMinimized->SetVisibility(ESlateVisibility::Visible);
}

void UUserManualWidget::MoveWidget()
{
	bMoveWidget = true;
}

void UUserManualWidget::RightPage1ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage2ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage3ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage4ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(4);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage5ButtonClicked()
{
	//WidgetSwitcher->SetActiveWidgetIndex(5);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage6ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(6);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage7ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(7);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::RightPage8ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(8);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage2ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage3ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage4ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage5ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage6ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(4);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage7ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(5);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage8ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(6);
	PlaySound(PageFlipSound);
}

void UUserManualWidget::LeftPage9ButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(7);
	PlaySound(PageFlipSound);
}

