// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/CompassIconWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DiverCharacter.h"
#include "Components/Image.h"
#include "Waypoint.h"
#include "Kismet/KismetMathLibrary.h"

void UCompassIconWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CompassMaterialInstance = UMaterialInstanceDynamic::Create(CompassMaterial, this);
	CompassMaterialInstance->SetScalarParameterValue(FName(TEXT("Offset")), 0.0f);
	CompassIconImage->SetBrushFromMaterial(CompassMaterialInstance);
}

void UCompassIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//Gets player character
	ADiverCharacter* DiverCharacter = Cast<ADiverCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (DiverCharacter != nullptr && ParentWaypoint != nullptr)
	{
		FRotator playerControlRotation = DiverCharacter->GetControlRotation();

		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(DiverCharacter->GetActorLocation(), ParentWaypoint->GetActorLocation());

		float scalar = playerControlRotation.Yaw - lookAt.Yaw;

		CompassMaterialInstance->SetScalarParameterValue(FName(TEXT("Offset")), scalar / 360);
	}

}

void UCompassIconWidget::SetCompassIconTexture(UTexture2D* Texture, AWaypoint* waypoint)
{
	ParentWaypoint = waypoint;
	CompassIconTexture = Texture;
	CompassMaterialInstance->SetTextureParameterValue(FName(TEXT("Icon")), CompassIconTexture);
}
