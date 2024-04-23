// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/JournalWidget.h"
#include "DiverCharacter.h"
#include "PhotographyCamera.h"
#include "Components/PhotographyComponent.h"
#include "Components/Image.h"
#include "DiverController.h"
#include "Engine/TextureRenderTarget2D.h"



void UJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PhotoGallery.SetNum(20);

	//Constructor helpers to find material
	PhotoGalleryMaterialBase = LoadObject<UMaterial>(nullptr, TEXT("/Game/ProjectAbyss/Art/Materials/PhotoMaterial.PhotoMaterial"), nullptr, LOAD_None, nullptr);

	for (int i = 0; i < 20; i++)
	{
		//Add each image to the array
		PhotoGalleryImages.Add(Cast<UImage>(GetWidgetFromName(FName("Image" + FString::FromInt(i + 1)))));
	}


	//PhotoGalleryMaterial = UMaterialInstanceDynamic::Create(PhotoGalleryMaterialBase, this);


	UpdatePhotoGallery();
}

void UJournalWidget::UpdatePhotoGallery()
{
	// Cast to the player controller
	ADiverController* PlayerController = Cast<ADiverController>(GetOwningPlayer());

	// Cast to diver character
	ADiverCharacter* DiverCharacter = Cast<ADiverCharacter>(PlayerController->GetPawn());

	// Update the photo gallery

	if (DiverCharacter)
	{
		if (DiverCharacter->CameraActor == nullptr)
		{
			// If the player does not have a camera, he can't upload pictures.
			return;
		}

		PhotoGallery = DiverCharacter->CameraActor->PhotographyCameraComponent->PhotoGallery;
	}

	for (int i = 0; i < PhotoGalleryImages.Num(); i++)
	{
		//Update each image
		UMaterialInstanceDynamic* PhotoGalleryMaterialTest = UMaterialInstanceDynamic::Create(PhotoGalleryMaterialBase, this);
		PhotoGalleryMaterialTest->SetTextureParameterValue("PhotoRender", Cast<UTexture>(PhotoGallery[i]));
		PhotoGalleryImages[i]->SetBrushFromMaterial(PhotoGalleryMaterialTest);
	}
}


void UJournalWidget::UpdateImages()
{

}
