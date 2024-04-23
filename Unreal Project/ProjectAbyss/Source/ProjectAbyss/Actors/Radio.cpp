// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Radio.h"
#include "MediaSoundComponent.h"

ARadio::ARadio()
{
	SoundComp = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Radio Sound"));
	SoundComp->SetupAttachment(RootComponent);
}

void ARadio::BeginPlay()
{
	Super::BeginPlay();

}

void ARadio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
