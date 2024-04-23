// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/InteractWidget.h"
#include "Components/TextBlock.h"
#include "InteractWidget.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InteractText->SetText(FText::FromString("Press (E) To Interact"));
}
