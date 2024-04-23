// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/GalleryListWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataObject.h"
#include "Components/Button.h"
#include "IndividualPhotoWidget.h"

void UGalleryListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ExpandPhotoButton->OnClicked.AddDynamic(this, &UGalleryListWidget::OnExpandPhotoButtonClicked);
}

void UGalleryListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    //Set the image
    DataObject = Cast<UDataObject>(ListItemObject);
    if (DataObject)
    {
        if (DataObject->GalleryDataStructTexture2D.PhotoGalleryTexture)
        {
            if (DataObject->GalleryDataStructTexture2D.PhotoGalleryMaterial)
            {
               PhotoGalleryMaterialTest = UMaterialInstanceDynamic::Create(DataObject->GalleryDataStructTexture2D.PhotoGalleryMaterial, this);
               PhotoGalleryMaterialTest->SetTextureParameterValue("PhotoRender", Cast<UTexture>(DataObject->GalleryDataStructTexture2D.PhotoGalleryTexture));
               Photo->SetBrushFromMaterial(PhotoGalleryMaterialTest);
            }

        }

    }
}

void UGalleryListWidget::OnExpandPhotoButtonClicked()
{
    //Creates the inidivdual photo widget and adds it to the viewport
    StoredIndividualPhotoWidget = CreateWidget<UIndividualPhotoWidget>(GetWorld(), IndividualPhotoWidgetClass);
    if (!StoredIndividualPhotoWidget) return;
    StoredIndividualPhotoWidget->SetupPhoto(PhotoGalleryMaterialTest);
    StoredIndividualPhotoWidget->AddToViewport();
}

