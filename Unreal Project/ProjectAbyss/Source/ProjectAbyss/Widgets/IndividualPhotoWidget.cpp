// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/IndividualPhotoWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PCUserWidget.h"
#include "Components/Overlay.h"

void UIndividualPhotoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PC = GetPC();

	//Bind the buttons
	CloseButton->OnClicked.AddDynamic(this, &UIndividualPhotoWidget::CloseButtonClicked);

	PC->StoredPhotoWidgets.Add(this);

	//Bind the border
	MoveBorder->OnMouseButtonDownEvent.BindUFunction(this, "MoveWidget");
}

void UIndividualPhotoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bMoveWidget == true)
	{
		UpdateBorder(this, Cast<UCanvasPanelSlot>(ContainingScaleBox->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	}

	FVector2D mouse;
	//Get mouse position after scaling/moving
	mouse = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	PreviousMouseX = mouse.X;
	PreviousMouseY = mouse.Y;
}

void UIndividualPhotoWidget::SetupPhoto(UMaterialInstanceDynamic* Photo)
{
	//Set the image
	StoredPhoto->SetBrushFromMaterial(Photo);
}

void UIndividualPhotoWidget::CloseButtonClicked()
{
	PC->StoredPhotoWidgets.Remove(this);
	PC->StoredPCWidget->SetFocus();

	RemoveFromParent();
}


void UIndividualPhotoWidget::MoveWidget()
{
	bMoveWidget = true;
}

FReply UIndividualPhotoWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bMoveWidget = false;

	return FReply::Handled();
}

