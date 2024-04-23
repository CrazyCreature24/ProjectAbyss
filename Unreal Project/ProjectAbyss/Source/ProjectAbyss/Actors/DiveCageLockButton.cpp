// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/DiveCageLockButton.h"
#include "Actors/DiveCage.h"
#include "Kismet/GameplayStatics.h"

ADiveCageLockButton::ADiveCageLockButton()
{
    TArray<AActor*> DiveCages;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiveCage::StaticClass(), DiveCages);

    if (DiveCages.Num() > 0)
    {
        DiveCage = Cast<ADiveCage>(DiveCages[0]);
    }

    //Attach this actor to the DiveCage
    if (DiveCage)
    {
        this->AttachToActor(DiveCage, FAttachmentTransformRules::KeepRelativeTransform);
    }
}

void ADiveCageLockButton::Interacted(ADiverCharacter* Player)
{
    Super::Interacted(Player);

    if (Player)
    {
        if (DiveCage)
        {
            DiveCage->ToggleLockCageDoor();
        }
    }

}
