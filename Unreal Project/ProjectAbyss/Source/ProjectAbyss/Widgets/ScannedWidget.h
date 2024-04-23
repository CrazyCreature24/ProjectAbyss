// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actors/Pawns/Creature.h"
#include "ScannedWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UScannedWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

	//Native Constructor
	virtual void NativeConstruct() override;

	//Native Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Function to set the creature
	void SetCreature(ACreature* Creature);

	//Start animation
	UFUNCTION()
	void StartAnimation();

	//Finish animation
	UFUNCTION()
	void FinishAnimation();

	ACreature* StoredCreature;

	//Uproperty bind for name
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name;

	class ADiverCharacter* Diver;

	FWidgetAnimationDynamicEvent OnAnimationFinished;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	class UWidgetAnimation* ScannedAnim;
};
