// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/InFocusWidget.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include "Math/Color.h"
#include "Creature.h"

void UInFocusWidget::SetCreatureName(ACreature* Fish)
{
	StoredCreature = Fish;

	if (Fish->bFirstTimeBonusAvailable == true)
	{
		name->SetColorAndOpacity(FLinearColor::Red);
		CameraBox->SetBrushFromTexture(Fish->RedCameraBoxTexture);
		name->SetText(FText::FromString("???"));
	}
	else
	{
		name->SetColorAndOpacity(FLinearColor::Blue);
		CameraBox->SetBrushFromTexture(Fish->BlueCameraBoxTexture);
		name->SetText(FText::FromString(Fish->Name));
	}
}

void UInFocusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInFocusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//Sets the textto the cooldown of the fish
	if (StoredCreature != nullptr)
	{
		//Gets timer from creature and sees if its on cooldown
		if (StoredCreature->bIsPictureCooldown)
		{
			float time = GetWorld()->GetTimerManager().GetTimerRemaining(StoredCreature->PictureCooldownTimerHandle);
			cooldown->SetText(FText::FromString(FString::FromInt(time)));
		}
		else
		{
			cooldown->SetText(FText::FromString("Ready"));
		}

	}
}
