// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/ControlsUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"

void UControlsUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    Cast<ADiverController>(GetOwningPlayer())->PlayerMoveDelegate.AddUniqueDynamic(this, &UControlsUserWidget::PlayerMove);
    Cast<ADiverController>(GetOwningPlayer())->PlayerInteractDelegate.AddUniqueDynamic(this, &UControlsUserWidget::PlayerInteract);
    InteractPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UControlsUserWidget::PlayerMove()
{
    if (WASDPanel->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
    {
        

        if (FKeyState* temp = Cast<ADiverController>(GetOwningPlayer())->PlayerInput->GetKeyState(FKey("W")))
        {
            if (temp->bDown)
            {
                WBorder->SetBrushColor(FColor::Green);
                bWBorder = true;
            }
        }
        if (FKeyState* temp = Cast<ADiverController>(GetOwningPlayer())->PlayerInput->GetKeyState(FKey("A")))
        {           
            if (temp->bDown)
            {
                ABorder->SetBrushColor(FColor::Green);
                bABorder = true;
            }

        }
        if (FKeyState* temp = Cast<ADiverController>(GetOwningPlayer())->PlayerInput->GetKeyState(FKey("S")))
        {
            if (temp->bDown)
            {
                SBorder->SetBrushColor(FColor::Green);
                bSBorder = true;
            }
        }
        if (FKeyState* temp = Cast<ADiverController>(GetOwningPlayer())->PlayerInput->GetKeyState(FKey("D")))
        {
            if (temp->bDown)
            {
                DBorder->SetBrushColor(FColor::Green);
                bDBorder = true;
            }
        }
        
        if (bWBorder && bABorder && bSBorder && bDBorder)
        {
            WASDPanel->SetVisibility(ESlateVisibility::Collapsed);
            InteractPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
    }
}

void UControlsUserWidget::PlayerInteract()
{
    if (InteractPanel->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
    {
        if (FKeyState* temp = Cast<ADiverController>(GetOwningPlayer())->PlayerInput->GetKeyState(FKey("E")))
        {
            if (temp->bDown)
            {
                InteractPanel->SetVisibility(ESlateVisibility::Collapsed);
                RemoveFromParent();
                
            }
        }
    }
}

FReply UControlsUserWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    

    return FReply::Handled();
}
