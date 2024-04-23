// Copyright Astralwater Interactive. All Rights Reserved.


#include "CameraViewfinder.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCameraViewfinder::NativeConstruct()
{
	Super::NativeConstruct();

	BrushAlpha = 0.0f;
	ShutterSpeed = 1.5f;
	MaxShutterAlpha = 255.0f;
}

void UCameraViewfinder::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BrushAlpha > 0.f)
	{
		Shutter->SetBrushTintColor(FSlateColor(FColor(0, 0, 0, BrushAlpha)));

		BrushAlpha -= InDeltaTime * 255.0f * ShutterSpeed;
	}
	

}

void UCameraViewfinder::ActivateShutter()
{
	BrushAlpha = MaxShutterAlpha;
}

void UCameraViewfinder::SetPictureCounter(int32 Counter)
{
	StoredCounter = Counter;
	// Set text to Film: Counter/Max without using printf
	PictureCounter->SetText(FText::FromString("Film: " + FString::FromInt(Counter) + "/20"));
}

void UCameraViewfinder::PlayOutOfFilmAnimation()
{
	PlayAnimation(Fade);
}
