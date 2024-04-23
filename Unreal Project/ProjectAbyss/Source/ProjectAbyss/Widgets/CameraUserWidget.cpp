// Copyright Astralwater Interactive. All Rights Reserved.


#include "CameraUserWidget.h"
#include "Components/Overlay.h"
#include "Camera/CameraActor.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "PCUserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"


//Nico mods
#include "Kismet/GameplayStatics.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"

void UCameraUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("MapID"); //Nico Mod


    PC = GetPC();


    TArray<AActor*> temparray;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), temparray);

    for (AActor* tempactor : temparray)
    {
        Boat = Cast<ABoat>(tempactor);
    }

    //
    PlayAnimation(MarkerAnim, 0.0f, 0);
    PlayAnimation(CaveAnim, 0.0f, 0);
    //
    MapPanel->SetCursor(EMouseCursor::GrabHand);

    //Button bindings
    MarkerButton->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::MarkerButtonClicked);
    CloseDetails->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::CloseDetailsClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::CloseButtonClicked);
    MinimizeButton->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::OnMinimize);
    CaveMarker->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::CaveButtonClicked);
    FossilYardButton->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::FossilButtonClicked);
    KelpForestButton->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::KelpButtonClicked);
    ShallowsButton->OnClicked.AddUniqueDynamic(this, &UCameraUserWidget::ShallowsButtonClicked);

    //FossilFinished.BindDynamic(this, &UCameraUserWidget::FossilAnimFinished);
    //KelpFinished.BindDynamic(this, &UCameraUserWidget::KelpAnimFinished);
    //ShallowsFinished.BindDynamic(this, &UCameraUserWidget::ShallowsAnimFinished);

    //BindToAnimationFinished(FossilAnim, FossilFinished);
    //BindToAnimationFinished(KelpAnim, KelpFinished);
    //BindToAnimationFinished(ShallowsAnim, ShallowsFinished);

    FossilBorder->OnMouseButtonDownEvent.BindUFunction(this,FName("FossilBorderClicked"));
    KelpBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("KelpBorderClicked"));
    ShallowsBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("ShallowsBorderClicked"));

    //Move map/widget bindings
    CamImage->OnMouseButtonDownEvent.BindUFunction(this, FName("MapClicked"));
    MoveBorderCamera->OnMouseButtonDownEvent.BindUFunction(this, FName("ClickBorderTest"));
    
    //Slider value changed binding
    MapSlider->OnValueChanged.AddUniqueDynamic(this, &UCameraUserWidget::MapSliderMoved);

    //Adds widget to the widget rotation (for controller support)
    if(PC->StoredPCWidget != nullptr)
    Cast<UPCUserWidget>(PC->StoredPCWidget)->AddToRotation(this, "Current");

    MapBorderSize = Cast<UCanvasPanelSlot>(MapBoundsBorder->Slot)->GetSize();
    
}

void UCameraUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    //Move widget handling
    if (bMoveWidget == true)
    {
        UpdateBorder(this, Cast<UCanvasPanelSlot>(CamOverlay->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
    }
    //Pan map handling
    if (PanningMap == true)
    {
        if(bAreaClicked == false)
            UpdateBorder(this, Cast<UCanvasPanelSlot>(MapPanel->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this), MapSlider->GetValue());
    }
    
    FVector2D Scale;

    Scale =  FVector2D(Boat->MapBoxExtent.X, Boat->MapBoxExtent.Y) / MapBorderSize;

    FVector2D Location;

    Location = (FVector2D(Boat->GetActorLocation().X, Boat->GetActorLocation().Y) - FVector2D(Boat->MapBoxOrigin.X, Boat->MapBoxOrigin.Y)) / (Scale * 2);

    Cast<UCanvasPanelSlot>(MarkerButton->Slot)->SetPosition(Location);
    MarkerButton->SetRenderTransformAngle(Boat->GetActorRotation().Yaw - 180);
    FVector2D mouse;
    //Get mouse position after scaling/moving
    mouse = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    PreviousMouseX = mouse.X;
    PreviousMouseY = mouse.Y;
}

void UCameraUserWidget::ClickBorderTest()
{
    //Bool to move widget
    bMoveWidget = true;

    Flicker(this);
}

FReply UCameraUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

    return FReply::Unhandled();
}

FReply UCameraUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    //Sets the cursor to grab hand if map is being panned
    if (PanningMap == true)
    {
        MapPanel->SetCursor(EMouseCursor::GrabHand);
    }

    //Sets all bools related to movement/scaling to false
    UnsetBools();

    return FReply::Handled();
}

FReply UCameraUserWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    //Zooms map
    ZoomMap(InMouseEvent);

    return FReply::Handled();
}

FReply UCameraUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    //Force a MouseUp Event when the bottom face gamepad button is released
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseUpEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonUpEvent(MouseUpEvent);
    }

    return FReply::Unhandled();
}

FReply UCameraUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == FKey("Tab"))
    {
        Cast<UPCUserWidget>(PC->StoredPCWidget)->Close();
    }

    //Force a MouseDown Event when the bottom face gamepad button is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseDownEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        //send the mouse event to the slate handler
        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseDownEvent);
    }
    //Force a MouseScrollUp Event when the left shoulder gamepad button is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_LeftShoulder)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();
        
        FPointerEvent MouseScrollEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::MouseScrollUp, 1, SlateApp.GetPlatformApplication()->GetModifierKeys());
        ZoomMap(MouseScrollEvent);
    }
    //Force a MouseScrollDown Event when the right shoulder gamepad button is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_RightShoulder)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseScrollEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::MouseScrollDown, -1, SlateApp.GetPlatformApplication()->GetModifierKeys());
        ZoomMap(MouseScrollEvent);
    }
    //Force the widget to close when the right face gamepad button is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right)
    {
        CloseButtonClicked();
    }

    return FReply::Unhandled();
}

void UCameraUserWidget::OnMinimize()
{
    //Minimize the widget and display the image indicating the widget is minimized
    PlayAnimation(MinimizeAnim);
    Cast<UPCUserWidget>(PC->StoredPCWidget)->MapIsMinimized->SetVisibility(ESlateVisibility::Visible);
}

void UCameraUserWidget::OnHotbarButtonClicked()
{
    //Displays the widget again and hides the image
    PlayAnimation(MinimizeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    Cast<UPCUserWidget>(PC->StoredPCWidget)->MapIsMinimized->SetVisibility(ESlateVisibility::Hidden);
}

void UCameraUserWidget::CloseButtonClicked()
{
    //Removes the widget from the taskbar
    Cast<UPCUserWidget>(PC->StoredPCWidget)->MapHotbar->SetVisibility(ESlateVisibility::Hidden);
    //Removes the widget from the screen
    RemoveFromParent();
    //Finds the widget in the widget rotation
    const FString* key = Cast<UPCUserWidget>(PC->StoredPCWidget)->WidgetRotation.FindKey(PC->StoredCameraWidget);
    if (key)
    {
        //Removes the widget from the widget rotation
        Cast<UPCUserWidget>(PC->StoredPCWidget)->WidgetRotation[*key] = nullptr;
    }
    //Sets the widget to null for garbage collection
    PC->StoredCameraWidget = nullptr;
    PC->StoredPCWidget->SetFocus();
}

void UCameraUserWidget::MarkerButtonClicked()
{
    //Plays animation to show details of the marker
    StopAnimation(MarkerAnim);
    PlayAnimation(DetailsAnim);
}

void UCameraUserWidget::CaveButtonClicked()
{
    //StopAnimation(CaveAnim);
    //PlayAnimation(DetailsAnim);
}

void UCameraUserWidget::CloseDetailsClicked()
{
    //Plays animation to hide details of the marker
    PlayAnimation(DetailsAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    
    PlayAnimation(MarkerAnim, 0.0f, 0);
    //PlayAnimation(MarkerAnim, 0.0f, 0);
}

void UCameraUserWidget::FossilButtonClicked()
{
    if (MapSlider->Value != 1.0f)
    {
        PlayAnimation(ZoomOutAnim, 0.0f, 1);

        FTimerHandle tempTimer;
        GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &UCameraUserWidget::FossilAnimFinished, 1.0f, false, 1.0f);
    }
    else
    {
        PlayAnimation(FossilAnim, 0.0f, 1);
    }

    bAreaClicked = true;
}

void UCameraUserWidget::KelpButtonClicked()
{
    if (MapSlider->Value != 1.0f)
    {
        PlayAnimation(ZoomOutAnim, 0.0f, 1);

        FTimerHandle tempTimer;
        GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &UCameraUserWidget::KelpAnimFinished, 1.0f, false, 1.0f);
    }
    else
    {
        PlayAnimation(KelpAnim, 0.0f, 1);
    }

    bAreaClicked = true;
}

void UCameraUserWidget::ShallowsButtonClicked()
{
    if (MapSlider->Value != 1.0f)
    {
        PlayAnimation(ZoomOutAnim, 0.0f, 1);

        FTimerHandle tempTimer;
        GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &UCameraUserWidget::ShallowsAnimFinished, 1.0f, false, 1.0f);
    }
    else
    {
        PlayAnimation(ShallowsAnim, 0.0f, 1);
    }

    bAreaClicked = true;
}

void UCameraUserWidget::FossilAnimFinished()
{
    PlayAnimation(FossilAnim, 0.0f, 1);
}

void UCameraUserWidget::KelpAnimFinished()
{
    PlayAnimation(KelpAnim, 0.0f, 1);
}

void UCameraUserWidget::ShallowsAnimFinished()
{
    PlayAnimation(ShallowsAnim, 0.0f, 1);
}

void UCameraUserWidget::FossilBorderClicked()
{
    PlayAnimation(FossilAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    bAreaClicked = false;
}

void UCameraUserWidget::KelpBorderClicked()
{
    PlayAnimation(KelpAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    bAreaClicked = false;
}

void UCameraUserWidget::ShallowsBorderClicked()
{
    PlayAnimation(ShallowsAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    bAreaClicked = false;
}

void UCameraUserWidget::MapSliderMoved(float Value)
{
    //Set scale of map based on slider value
    MapPanel->SetRenderScale(FVector2D(Value, Value));

    //Get map slot
    UCanvasPanelSlot* temp = Cast<UCanvasPanelSlot>(MapPanel->Slot);

    //Change position of map for scaling
    temp->SetPosition(FVector2D(FMath::Clamp(temp->GetPosition().X, 0 - (((MapSlider->GetValue() - 1) * 10) * 64), 0 + (((MapSlider->GetValue() - 1) * 10) * 64)), FMath::Clamp(temp->GetPosition().Y, 32 - (((MapSlider->GetValue() - 1) * 10) * 34), 32 + (((MapSlider->GetValue() - 1) * 10) * 34))));
}

float UCameraUserWidget::GetSliderVal()
{
    return MapSlider->Value;
}

FEventReply UCameraUserWidget::MapClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
    //Sets the bool to allow panning
    PanningMap = true;

    //Sets the cursor to a closed hand to show panning is working
    MapPanel->SetCursor(EMouseCursor::GrabHandClosed);

    Flicker(this);

    return FEventReply(false);
}

void UCameraUserWidget::ZoomMap(const FPointerEvent& InMouseEvent)
{
    if (bAreaClicked == false)
    {

        UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("MapZoomID");


        FVector2D size;

        size = MapPanel->GetRenderTransform().Scale;

        size.X += InMouseEvent.GetWheelDelta() * 0.1; //38
        size.X = FMath::Clamp(size.X, 1, 2.5);

        size.Y += InMouseEvent.GetWheelDelta() * 0.1;//20
        size.Y = FMath::Clamp(size.Y, 1, 2.5);

        MapSlider->SetValue(size.X);

        MapPanel->SetRenderScale(size);

        UCanvasPanelSlot* temp = Cast<UCanvasPanelSlot>(MapPanel->Slot);

        temp->SetPosition(FVector2D(FMath::Clamp(temp->GetPosition().X, 0 - (((MapSlider->GetValue() - 1) * 10) * 64), 0 + (((MapSlider->GetValue() - 1) * 10) * 64)), FMath::Clamp(temp->GetPosition().Y, 32 - (((MapSlider->GetValue() - 1) * 10) * 34), 32 + (((MapSlider->GetValue() - 1) * 10) * 34))));
    }
}

