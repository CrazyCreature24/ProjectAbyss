// Copyright Astralwater Interactive. All Rights Reserved.



#include "Widgets/ScannedWidget.h"
#include "Components/TextBlock.h"
#include "DiverCharacter.h"

//Native Constructor
void UScannedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnAnimationFinished.BindDynamic(this, &UScannedWidget::FinishAnimation);

	//On animation finished
	BindToAnimationFinished(ScannedAnim, OnAnimationFinished);


}

void UScannedWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

//Function to set the creature
void UScannedWidget::SetCreature(ACreature* Creature)
{
	StoredCreature = Creature;
	if (StoredCreature == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("StoredCreature is null in ScannedWidget"));
		return;
	}

	if (StoredCreature->bFirstTimeBonusAvailable)
	{
		//Set the name of the creature
		Name->SetText(FText::FromString("???"));
	}
	else
	{
		//Set the name of the creature
		Name->SetText(FText::FromString(StoredCreature->Name));
	}
}

void UScannedWidget::StartAnimation()
{
	//Play the animation
    PlayAnimation(ScannedAnim, 0.0f, 1);
}

void UScannedWidget::FinishAnimation()
{
    //Stop the animation
	if (StoredCreature == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("StoredCreature is null in ScannedWidget"));
		return;
	}
	StoredCreature->HideScannedWidget();
}
