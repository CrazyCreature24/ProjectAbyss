// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PCTerminal.h"
#include "Creature.h"
#include "ProfileCardWidget.generated.h"

class UOverlay;

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UProfileCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets the profile name
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* ScientificName;

	// Sets the weight
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Weight;

	// Sets the length
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Length;

	// Sets the habitat
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Habitat;

	// Sets the diet
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Diet;

	// Sets the physical description
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* PhysicalDescription;

	// Sets the behavior description
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Behaviors;

	// Sets the editable text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UEditableText* EditableName;

	// Sets the profile image
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* FishImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* BorderEdge;

	// Other notes multi line editable text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UMultiLineEditableText* OtherNotes;

	UFUNCTION()
	void BorderClicked();

	void PopulateProfileCard(int CollectionIndex);

	//Native construct override
	virtual void NativeConstruct() override;

	//Tick override
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;


	UPROPERTY(EditAnywhere, meta=(BindWidget))
		UOverlay* ProfileOverlay;

	ACreature* StoredCreature;
	
	int StoredCollectionIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APCTerminal* StoredTerminal;
};
