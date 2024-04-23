// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/PCTerminal.h"
#include "CameraUserWidget.h"
#include "PCUserWidget.h"
#include "MissionUserWidget.h"
#include "DiverCharacter.h"
#include "DiverController.h"
#include "UserManualWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/PointLightComponent.h"


#include "Components/AudioComponent.h"

#include "InputAction.h"


APCTerminal::APCTerminal()
{
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> texture(TEXT("/ Script / Engine.TextureRenderTarget2D'/Game/ProjectAbyss/Blueprints/Widgets/StuffForCamWidget/CamTarget.CamTarget'"));
    Texture = texture.Object;

    PCNotificationWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("PC Notification Widget");
    PCNotificationWidgetComponent->SetupAttachment(RootComponent);
    PCNotificationWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    PCNotificationWidgetComponent->SetVisibility(false);

    NotificationLightComponent = CreateDefaultSubobject<UPointLightComponent>("Notification Light");
    NotificationLightComponent->Intensity = 2000.0f;


    ComputerSounds = CreateDefaultSubobject<UAudioComponent>("Computer Audio");

}

void APCTerminal::BeginPlay()
{
    Super::BeginPlay();
    
    NotificationLightComponent->Intensity = 2000.0f;

}

void APCTerminal::Interacted(ADiverCharacter* Player)
{
    AInteractableActor::Interacted(Player);

    StoredPCWidget = CreateWidget<UPCUserWidget>(Cast<APlayerController>(Player->Controller), PCWidget);
    //StoredMissionWidget = CreateWidget<UMissionUserWidget>(Cast<APlayerController>(Player->Controller), MissionWidget);
    //StoredCameraWidget = CreateWidget<UCameraUserWidget>(Cast<APlayerController>(Player->Controller), CameraWidget);
    UMissionLogSubsystem* QuestLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();


    QuestLog->CallObjectiveID("TestIDPC");

    //UUserWidget* widget;
    FInputModeUIOnly test;
 
        
        

        StoredPCWidget->AddToViewport();
        //Cast to player controller

        ADiverController* PC = Cast<ADiverController>(Player->GetController());


        if (PC)
        {
        PC->SetInputMode(test);
        PC->bShowMouseCursor = true;
        PC->FlushPressedKeys();
        }
        
        
        
        
}

void APCTerminal::ToggleNotification(bool isNotifOn)
{
    if (isNotifOn)
    {
        NotificationLightComponent->Intensity = 2000.0f;
        PCNotificationWidgetComponent->SetVisibility(true);
    }
    else
    {
        NotificationLightComponent->Intensity = 0.0f;
        PCNotificationWidgetComponent->SetVisibility(false);
    }

}
