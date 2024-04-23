// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FishFinderWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UFishFinderWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* FishCountText;

    // Native construct
	virtual void NativeConstruct() override;

	//Set the fish count
	UFUNCTION(BlueprintCallable, Category = "FishFinder")
	void SetFishCount(int FishCount);

};
