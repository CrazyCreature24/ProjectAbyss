// Copyright Astralwater Interactive. All Rights Reserved.

#include "Widgets/CollectionJournalWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TileView.h"
#include "Components/TextBlock.h"
#include "DataObject.h"
#include "CollectionSystemBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <DiverController.h>
#include "PhotographyCamera.h"
#include "PhotographyComponent.h"
#include <MainGameMode.h>
#include "SaveLoadSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectAbyssSaveGame.h>
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "MissionObjectiveIDComponent.h"
#include "PCUserWidget.h"
#include "Blueprint/AsyncTaskDownloadImage.h" //Might not need
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Texture2D.h"
//#include "Kismet/KismetArrayLibrary.h"

UCollectionJournalWidget::UCollectionJournalWidget()
{
    JournalPhotosFilePath = FString(UKismetSystemLibrary::GetProjectSavedDirectory() + FString("JournalPhotos"));

}

void UCollectionJournalWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PC = GetPC();

   // JournalPhotosFilePath = FString(UKismetSystemLibrary::GetProjectSavedDirectory() + FString("JournalPhotos"));

    MoveBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("MoveWidget"));
    CloseButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::Close);

    FishButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::FishTabClicked);
    PlantButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::PlantTabClicked);
    SquidButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::SquidTabClicked);
    CrabButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::CrabTabClicked);
    GalleryButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::GalleryTabClicked);
    UploadButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::UploadButtonClicked);
    MinimizeButton->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::MinimizeButtonClicked);

    GalleryNext->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::GalleryNextButtonClicked);
    GalleryPrev->OnPressed.AddUniqueDynamic(this, &UCollectionJournalWidget::GalleryPrevButtonClicked);


    JournalTabs->SetActiveWidgetIndex(4);

    SetFiles();
    RefreshTabs();
    RefreshGallery();

    //Constructor helpers to find material
    PhotoGalleryMaterialBase = LoadObject<UMaterial>(nullptr, TEXT("/Game/ProjectAbyss/Art/Materials/PhotoMaterial.PhotoMaterial"), nullptr, LOAD_None, nullptr);

    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("JournalID"); //Nico Mod for missions


}

void UCollectionJournalWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(bMoveWidget == true)
    {
        UpdateBorder(this, Cast<UCanvasPanelSlot>(MovableOverlay->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
    }

    FVector2D mouse;
    //Get mouse position after scaling/moving
    mouse = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    PreviousMouseX = mouse.X;
    PreviousMouseY = mouse.Y;
}

FReply UCollectionJournalWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Flicker(this);

    return FReply::Unhandled();
}

FReply UCollectionJournalWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UnsetBools();

    return FReply::Handled();
}

void UCollectionJournalWidget::FishTabClicked()
{
    JournalTabs->SetActiveWidgetIndex(0);
    RefreshTabs();
}

void UCollectionJournalWidget::PlantTabClicked()
{
    JournalTabs->SetActiveWidgetIndex(1);
    RefreshTabs();
}

void UCollectionJournalWidget::SquidTabClicked()
{
    JournalTabs->SetActiveWidgetIndex(2);
    RefreshTabs();
}

void UCollectionJournalWidget::CrabTabClicked()
{
    JournalTabs->SetActiveWidgetIndex(3);
    RefreshTabs();
}

void UCollectionJournalWidget::GalleryTabClicked()
{
    JournalTabs->SetActiveWidgetIndex(4);
    RefreshGallery();
}

void UCollectionJournalWidget::UploadButtonClicked()
{
    //Updates the photogallery. 

    // Cast to the player controller
    ADiverController* PlayerController = Cast<ADiverController>(GetOwningPlayer());

    // Cast to diver character
    ADiverCharacter* Player = Cast<ADiverCharacter>(PlayerController->GetPawn());


    MoneyEarnedFromLastUpload = 0;

    AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

   

    //Get the collection photography subsystem
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectionSystemBase::StaticClass(), FoundActors);
    ACollectionSystemBase* TempCollectionSystem = Cast<ACollectionSystemBase>(FoundActors[0]);

    if (Player->CameraActor == nullptr)
    {
        // If the player does not have a camerea, he can't upload pictures.
        UE_LOG(LogTemp, Warning, TEXT("No camera found"));
        return;
    }
    UPhotographyComponent* photoComponent = Player->CameraActor->PhotographyCameraComponent;

    if (FoundActors.Num() > 0)
    {
        TempCollectionSystem->LoadCollectionData(); //This does NOTHING


        //for (const auto& CapturedFishPair : photoComponent->CapturedFish)
        //{
        //    ACreature* CapturedFish = CapturedFishPair.Key;

        //    // if the particular fish type has not been captured yet, change bool on CollectionSystemBase to true
        //    for (int i = 0; i < TempCollectionSystem->CreatureCollectionArray.Num(); i++)
        //    {                           
        //        //If the fish has been captured before, and is the same as the one in the collection system, add it to the collection
        //        if (TempCollectionSystem->CreatureCollectionArray[i].Creature != nullptr && CapturedFish->CollectionIndex == TempCollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CollectionIndex)
        //        {

        //                // If that fish has not been captured before, add it to the collection
        //                if (TempCollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable)
        //                {

        //                    TempCollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable = false; //Saves too well

        //                    CapturedFish->SetCaptured();

        //                    AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

        //                    if (MyGameState)
        //                    {
        //                        if (MyGameState->GetPlayerHasDied())
        //                        {
        //                            GameMode->Money += CapturedFish->FirstTimeBonus / 2;
        //                            MoneyEarnedFromLastUpload += CapturedFish->FirstTimeBonus / 2;
        //                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from capturing %s for the first time: %d"), *CapturedFish->Name, CapturedFish->FirstTimeBonus));
        //                        }
        //                        else
        //                        {
        //                            GameMode->Money += CapturedFish->FirstTimeBonus;
        //                            MoneyEarnedFromLastUpload += CapturedFish->FirstTimeBonus;
        //                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from capturing %s for the first time: %d"), *CapturedFish->Name, CapturedFish->FirstTimeBonus));
        //                        }
        //                    }
        //                    

        //                }
        //                else
        //                {
        //                    AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

        //                    if (MyGameState)
        //                    {
        //                        // If the fish has been captured before, add the money earned from recapturing it to the total money earned
        //                        if (MyGameState->GetPlayerHasDied())
        //                        {
        //                            GameMode->Money += CapturedFish->MoneyValue / 2;
        //                            MoneyEarnedFromLastUpload += CapturedFish->MoneyValue / 2;
        //                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from recapturing %s: %d"), *CapturedFish->Name, CapturedFish->MoneyValue));
        //                        }
        //                        else
        //                        {
        //                            GameMode->Money += CapturedFish->MoneyValue;
        //                            MoneyEarnedFromLastUpload += CapturedFish->MoneyValue;
        //                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from recapturing %s: %d"), *CapturedFish->Name, CapturedFish->MoneyValue));
        //                        }
        //                    }
        //                }
        //                CapturedFish->ObjectiveIDsComponent->BroadcastObjectiveIDs();
        //        }
        //    }

        //}

        for (const TSubclassOf<ACreature> CapturedFish : photoComponent->FishCapturedNico)
        {
            ACreature* CapFish = Cast<ACreature>(CapturedFish->GetDefaultObject());


                // if the particular fish type has not been captured yet, change bool on CollectionSystemBase to true
                for (int i = 0; i < TempCollectionSystem->CreatureCollectionArray.Num(); i++)
                {
                    //If the fish has been captured before, and is the same as the one in the collection system, add it to the collection
                    if (TempCollectionSystem->CreatureCollectionArray[i].Creature != nullptr && CapFish->CollectionIndex == TempCollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CollectionIndex)
                    {

                        // If that fish has not been captured before, add it to the collection
                        if (TempCollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable)
                        {

                            TempCollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable = false; //Saves too well
                            TempCollectionSystem->CreatureCollectionArray[i].bFirstTimeBonusAvailable = false; //Saves too well



                            //CapFish->SetCaptured(); //Not useful here / crashes

                            AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

                            if (MyGameState)
                            {
                                if (MyGameState->GetPlayerHasDied())
                                {
                                    GameMode->Money += CapFish->FirstTimeBonus / 2;
                                    MoneyEarnedFromLastUpload += CapFish->FirstTimeBonus / 2;
                                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from capturing %s for the first time: %d"), *CapFish->Name, CapFish->FirstTimeBonus));
                                }
                                else
                                {
                                    GameMode->Money += CapFish->FirstTimeBonus;
                                    MoneyEarnedFromLastUpload += CapFish->FirstTimeBonus;
                                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from capturing %s for the first time: %d"), *CapFish->Name, CapFish->FirstTimeBonus));
                                }
                            }


                        }
                        else
                        {
                            AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

                            if (MyGameState)
                            {
                                // If the fish has been captured before, add the money earned from recapturing it to the total money earned
                                if (MyGameState->GetPlayerHasDied())
                                {
                                    GameMode->Money += CapFish->MoneyValue / 2;
                                    MoneyEarnedFromLastUpload += CapFish->MoneyValue / 2;
                                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from recapturing %s: %d"), *CapFish->Name, CapFish->MoneyValue));
                                }
                                else
                                {
                                    GameMode->Money += CapFish->MoneyValue;
                                    MoneyEarnedFromLastUpload += CapFish->MoneyValue;
                                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Money earned from recapturing %s: %d"), *CapFish->Name, CapFish->MoneyValue));
                                }
                            }
                        }
                        CapFish->ObjectiveIDsComponent->BroadcastObjectiveIDs(GetWorld());
                    }
                }

            
        }
        TempCollectionSystem->SaveCollectionData(); //This does NOTHING

        USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();
        SaveLoad->SaveCollectionSystemlData(TempCollectionSystem);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No collection system found!"));
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Total money earned from this upload: %d"), MoneyEarnedFromLastUpload));

    MoneyEarnedText->SetText(FText::FromString(FString::Printf(TEXT("Total money earned from this upload: %d"), MoneyEarnedFromLastUpload)));

    // Update the photo gallery

  //  if (Player)
  //  {
  //      if (Player->CameraActor == nullptr)
  //      {
  //          // If the player does not have a camerea, he can't upload pictures.
  //          UE_LOG(LogTemp, Warning, TEXT("No camera found"));
  //          return;
  //      }

  //      //Loads the photo gallery from the save game
  //      USaveLoadSubsystem* SaveLoadSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveLoadSubsystem>();
  //      UProjectAbyssSaveGame* SaveGame = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
  //      PhotoGallery = SaveGame->SavedPhotoGallery;

  //      //Add the photos from the temp gallery to the photo gallery. Keep the original photos in the photo gallery.
  //      TArray<class UTextureRenderTarget2D*> tempGallery = Player->CameraActor->PhotographyCameraComponent->PhotoGallery;
  //      for (int i = 0; i < tempGallery.Num(); i++)
  //      {
  //          PhotoGallery.Add(tempGallery[i]);
		//}

  //      //Get save game subsystem USaveLoadSubsystem
  //      SaveLoadSubsystem->SavePhotoGallery(PhotoGallery);
  //  }   

    if (photoComponent)
    {
        photoComponent->ResetCamera();
    }

    //Get all current creatures in the world
    TArray<AActor*> FoundActors1;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACreature::StaticClass(), FoundActors1);

    //For loop for all creatures in the world
    for (int i = 0; i < FoundActors1.Num(); i++)
    {
		ACreature* Creature = Cast<ACreature>(FoundActors1[i]);
        if (Creature)
        {
            for (int j = 0; j < TempCollectionSystem->CreatureCollectionArray.Num(); j++)
			{
				if (TempCollectionSystem->CreatureCollectionArray[j].Creature != nullptr && Creature->CollectionIndex == TempCollectionSystem->CreatureCollectionArray[j].Creature.GetDefaultObject()->CollectionIndex)
				{
                    if (TempCollectionSystem->CreatureCollectionArray[j].Creature.GetDefaultObject()->bFirstTimeBonusAvailable == false)
					Creature->SetCaptured();
				}
			}
		}
	}

    //
    IFileManager& FileManager = IFileManager::Get();

    TArray<FString> CameraFiles;


    //FileManager.FindFiles(Files, FString(UKismetSystemLibrary::GetProjectSavedDirectory() + FString("\\CameraPhotos")).GetCharArray().GetData(), NULL);
    FileManager.FindFiles(CameraFiles, photoComponent->CameraPhotosFilePath.GetCharArray().GetData(), NULL);

    for (FString PhotoFileName : CameraFiles)
    {
        FString PhotoSource =  photoComponent->CameraPhotosFilePath + "/" + PhotoFileName;
        FString PhotoDestination = JournalPhotosFilePath + "/" + PhotoFileName;
        //FileManager.Move(JournalPhotosFilePath.GetCharArray().GetData(), FString(photoComponent->CameraPhotosFilePath + PhotoFileName).GetCharArray().GetData(), true, true, true);
       // FileManager.Move(FString(JournalPhotosFilePath + PhotoFileName).GetCharArray().GetData(), FString(photoComponent->CameraPhotosFilePath + PhotoFileName).GetCharArray().GetData(), true, true, true);
        FileManager.Move(PhotoDestination.GetCharArray().GetData(), PhotoSource.GetCharArray().GetData(), true, true, true);
        //FString(JournalPhotosFilePath + PhotoFileName).GetCharArray().GetData();
    }

    //
    SetFiles(); 
    GalleryIndex = 1;
    RefreshGallery();

    AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

    if (MyGameState)
    {
        MyGameState->ResetPlayerState();
    }

}

void UCollectionJournalWidget::GalleryNextButtonClicked()
{
    GalleryIndex++;
    GalleryIndex = FMathf::Clamp(GalleryIndex, 1, MaxGalleryPages);
    RefreshGallery();
}

void UCollectionJournalWidget::GalleryPrevButtonClicked()
{
    GalleryIndex--;
    GalleryIndex = FMathf::Clamp(GalleryIndex, 1, MaxGalleryPages);
    RefreshGallery();

}

void UCollectionJournalWidget::SetFiles()
{
    IFileManager& FileManager = IFileManager::Get();
    PhotoFiles.Empty();
    FileManager.FindFiles(PhotoFiles, JournalPhotosFilePath.GetCharArray().GetData(), NULL);
    //PhotoFiles.revr
    MaxGalleryPages = PhotoFiles.Num() / MaxPhotosInGallery;
    if (PhotoFiles.Num() % MaxPhotosInGallery > 0)
        MaxGalleryPages ++;
}

void UCollectionJournalWidget::UpdateGalleryPageText()
{
    if (MaxGalleryPages == 0)
    {
        GalleryNext->SetVisibility(ESlateVisibility::Hidden);
        GalleryPrev->SetVisibility(ESlateVisibility::Hidden);
        GalleryPageText->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        GalleryNext->SetVisibility(ESlateVisibility::Visible);
        GalleryPrev->SetVisibility(ESlateVisibility::Visible);
        GalleryPageText->SetVisibility(ESlateVisibility::Visible);
    }

    FString temp = FString::FromInt(GalleryIndex) + "/" + FString::FromInt(MaxGalleryPages);
    GalleryPageText->SetText(FText::FromString(temp));
}

void UCollectionJournalWidget::RefreshTabs()
{
    FishTileView->ClearListItems();
    PlantTileView->ClearListItems();
    SquidTileView->ClearListItems();
    CrabTileView->ClearListItems();

    //Find the collection object in world using foundActor
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectionSystemBase::StaticClass(), FoundActors);
    if (FoundActors.Num() == 0)
    {
        return;
    }
    ACollectionSystemBase* CollectionSystem = Cast<ACollectionSystemBase>(FoundActors[0]);

        

    // For loop for all creatures in the collection system
    for (int i = 0; i < CollectionSystem->CreatureCollectionArray.Num(); i++)
    {
        if (CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject() != nullptr)
        {
            UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

            // If the creature is a fish
            if (CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CreatureType == ECreatureType::CT_Fish)
            {
                tempobject->Init(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->IconImage, FText::FromString(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->Name), !CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable, CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CollectionIndex);
                // Add the creature to the fish tile view
                FishTileView->AddItem(tempobject);
            }
            // If the creature is a plant
            else if (CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CreatureType == ECreatureType::CT_Plant)
            {
                tempobject->Init(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->IconImage, FText::FromString(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->Name), !CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable, CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CollectionIndex);
                // Add the creature to the plant tile view
                PlantTileView->AddItem(tempobject);
            }
            // If the creature is a remnant
            else if (CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CreatureType == ECreatureType::CT_Remnants)
            {
                tempobject->Init(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->IconImage, FText::FromString(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->Name), !CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable, CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CollectionIndex);
                // Add the creature to the squid tile view
                SquidTileView->AddItem(tempobject);
            }
            // If the creature is a bowling ball.
            else if (CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CreatureType == ECreatureType::CT_BowlingBalls)
            {
                tempobject->Init(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->IconImage, FText::FromString(CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->Name), !CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable, CollectionSystem->CreatureCollectionArray[i].Creature.GetDefaultObject()->CollectionIndex);
                // Add the creature to the crab tile view
                CrabTileView->AddItem(tempobject);
            }
        }
	}
}
void UCollectionJournalWidget::RefreshGallery()
{
	GalleryTileView->ClearListItems();
    //PhotoGallery.Empty();
    PhotoGalleryT2D.Empty();


    //SetFiles(); //Move this to upload and begin play

    //for (int i = 0; i < Files.Num(); i++ )
    //for (int i = PhotoFiles.Num() - 1; i > -1; i--) //Comment Later
    //{

    //    FString FileName = PhotoFiles[i];
    //    FString PhotoFilePath = JournalPhotosFilePath + "/" + FileName;

    //   // UAsyncTaskDownloadImage::DownloadImage(PhotoFilePath);

    //    UTexture2D* SavedPhoto = UKismetRenderingLibrary::ImportFileAsTexture2D(this, PhotoFilePath);

    //    PhotoGalleryT2D.Add(SavedPhoto);

    //    UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

    //    	tempobject->Init(PhotoGalleryMaterialBase,SavedPhoto);
    //    	GalleryTileView->AddItem(tempobject);
    //        if (i  < PhotoFiles.Num() - 8)
    //            break;
    //}

    //for (int i = (GalleryIndex-1) * MaxPhotosInGallery; i < (GalleryIndex - 1) * MaxPhotosInGallery + MaxPhotosInGallery; i++)
    //{
    //    if (i >= PhotoFiles.Num())
    //        break;

    //    FString FileName = PhotoFiles[i];
    //    FString PhotoFilePath = JournalPhotosFilePath + "/" + FileName;

    //    UTexture2D* SavedPhoto = UKismetRenderingLibrary::ImportFileAsTexture2D(this, PhotoFilePath);

    //    UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());
    //    tempobject->Init(PhotoGalleryMaterialBase, SavedPhoto);
    //    GalleryTileView->AddItem(tempobject);
    //}   
    
    int tempindex = (PhotoFiles.Num() - 1) - ((GalleryIndex - 1) * 4);
    int tempLimit = ((PhotoFiles.Num() - 1) - ((GalleryIndex - 1) * 4)) - MaxPhotosInGallery;

    for (tempindex; tempindex > tempLimit; tempindex--)
    {
        if (tempindex < 0)
            break;

        FString FileName = PhotoFiles[tempindex];
        FString PhotoFilePath = JournalPhotosFilePath + "/" + FileName;

        UTexture2D* SavedPhoto = UKismetRenderingLibrary::ImportFileAsTexture2D(this, PhotoFilePath);

        UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());
        tempobject->Init(PhotoGalleryMaterialBase, SavedPhoto);
        GalleryTileView->AddItem(tempobject);
    }

    UpdateGalleryPageText();

}

void UCollectionJournalWidget::MoveWidget()
{
    Flicker(this);

    bMoveWidget = true;
}

void UCollectionJournalWidget::Close()
{
    Cast<UPCUserWidget>(PC->StoredPCWidget)->JournalHotbar->SetVisibility(ESlateVisibility::Hidden);

    if (Cast<UPCUserWidget>(PC->StoredPCWidget)->StoredProfileCard != nullptr)
    {
        Cast<UPCUserWidget>(PC->StoredPCWidget)->StoredProfileCard->RemoveFromParent();
        Cast<UPCUserWidget>(PC->StoredPCWidget)->StoredProfileCard = nullptr;
	}

    for (int i = 0; i < PC->StoredPhotoWidgets.Num(); i++)
    {
        PC->StoredPhotoWidgets[i]->RemoveFromParent();
        PC->StoredPhotoWidgets[i] = nullptr;
	}
    PC->StoredPhotoWidgets.Empty();

    PC->StoredCollectionWidget = nullptr;
    PC->StoredPCWidget->SetFocus();

    RemoveFromParent();
}

void UCollectionJournalWidget::OnHotbarButtonClicked()
{
    //Displays the widget again and hides the image
    Cast<UPCUserWidget>(PC->StoredPCWidget)->JournalIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    PlayAnimation(MinimizeAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UCollectionJournalWidget::MinimizeButtonClicked()
{
    //Play animation to hide widget
    PlayAnimation(MinimizeAnimation);
    Cast<UPCUserWidget>(PC->StoredPCWidget)->JournalIsMinimized->SetVisibility(ESlateVisibility::Visible);
}
