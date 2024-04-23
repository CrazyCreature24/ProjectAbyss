// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraViewfinder.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UCameraViewfinder : public UUserWidget
{
	GENERATED_BODY()

	public:
		void NativeConstruct() override;

		void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

		void ActivateShutter();

		void SetPictureCounter(int32 Counter);

		void PlayOutOfFilmAnimation();

		float ShutterSpeed;

		float MaxShutterAlpha;

		float BrushAlpha;

		class APhotographyCamera* Camera;

		UPROPERTY(meta = (BindWidget))
		class UImage* Shutter;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* PictureCounter;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* OutOfFilm;

		//Transient animation bind
		UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Fade;

		int32 StoredCounter;

};

/*
ChangeLog:
---------------------------- -
Date : March 14th
Name : Alexander McTernan
Summary : Class Created and it adds black image on Shutter activation that slowly fades away.
---------------------------- -
*/