// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/ProfileCardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CollectionSystemBase.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "CollectionJournalWidget.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "PCUserWidget.h"
#include "Components/Button.h"
#include "SaveLoadSubsystem.h"
#include "ProjectAbyssSaveGame.h"
#include "Components/MultiLineEditableText.h"

void UProfileCardWidget::BorderClicked()
{
    Cast<UPCUserWidget>(StoredTerminal->StoredPCWidget)->StoredProfileCard = nullptr;
    RemoveFromParent();
}

void UProfileCardWidget::PopulateProfileCard(int CollectionIndex)
{
    //Find the collection object in world using foundActor
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectionSystemBase::StaticClass(), FoundActors);
    ACollectionSystemBase* CollectionSystem = Cast<ACollectionSystemBase>(FoundActors[0]);

    // Sets the collection index to the index of the creature in the collection array
    if (CollectionSystem->CreatureCollectionArray[CollectionIndex - 1].Creature.GetDefaultObject() != nullptr)
	{
		StoredCreature = Cast<ACreature>(CollectionSystem->CreatureCollectionArray[CollectionIndex - 1].Creature.GetDefaultObject());
	}
	else
	{
		StoredCreature = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("StoredCreature is null. Array index is likely wrong. Check to ensure creatures put in match the array count."));
	}

    //FText::FromString("#" + FString::FromInt(DataObject->JournalDataStruct.JournalID)
    //Sets the variables to every widget in the profile card going from FString to UTextBlock
    if (StoredCreature)
    {
        FishImage->SetBrushFromTexture(StoredCreature->ProfileImage);
        ScientificName->SetText(FText::FromString("Scientific Name: " + StoredCreature->ScientificName));
        Weight->SetText(FText::FromString("Weight: " + StoredCreature->Weight));
        Length->SetText(FText::FromString("Length: " + StoredCreature->Length));
        Habitat->SetText(FText::FromString("Habitat: " + StoredCreature->Habitat));
        Diet->SetText(FText::FromString("Diet: " + StoredCreature->Diet));
        PhysicalDescription->SetText(FText::FromString("Physical Description: " + StoredCreature->PhysicalDescription));
        Behaviors->SetText(FText::FromString("Behaviors: " + StoredCreature->Behaviors));
        EditableName->SetText(FText::FromString( StoredCreature->Name));

        USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();
        FProfileCardSavedInfo SaveProfileCardInfo;
        if (SaveLoad->GetProfileCardInfo(CollectionIndex, SaveProfileCardInfo))
        {
            EditableName->SetText(FText::FromString(SaveProfileCardInfo.FishName));
            OtherNotes->SetText(FText::FromString(SaveProfileCardInfo.Notes));
        }

    }
    StoredCollectionIndex = CollectionIndex;
}

void UProfileCardWidget::NativeConstruct()
{
    Super::NativeConstruct();
    //Find the collection object in world using foundActor
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), FoundActors);
    StoredTerminal = Cast<APCTerminal>(FoundActors[0]);

    //Bind the border click
    BorderEdge->OnClicked.AddDynamic(this, &UProfileCardWidget::BorderClicked);
}   

void UProfileCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
 
    //Sets location of the profile card to the collection widget
    if (StoredTerminal->StoredCollectionWidget != nullptr)
    Cast<UCanvasPanelSlot>(ProfileOverlay->Slot)->SetPosition(FVector2D(0.0f,30.0f) + Cast<UCanvasPanelSlot>(Cast<UCollectionJournalWidget>(StoredTerminal->StoredCollectionWidget)->MovableOverlay->Slot)->GetPosition());
}

void UProfileCardWidget::NativeDestruct()
{
    USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();
    SaveLoad->SaveJournalEntry(this);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectionSystemBase::StaticClass(), FoundActors);
    ACollectionSystemBase* CollectionSystem = Cast<ACollectionSystemBase>(FoundActors[0]);
    
    CollectionSystem->UpdateInfoOfFishInWorld(StoredCollectionIndex, EditableName->Text.ToString());

    if (Cast<UPCUserWidget>(StoredTerminal->StoredPCWidget) != nullptr)
    {
        Cast<UCollectionJournalWidget>(StoredTerminal->StoredCollectionWidget)->RefreshTabs();
    }

    SaveLoad->SaveCollectionSystemlData(CollectionSystem);

    Super::NativeDestruct();

}


