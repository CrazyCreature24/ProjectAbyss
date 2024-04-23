// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Equipment/SoundDecoy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "InventoryComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "DiverCharacter.h"

ASoundDecoy::ASoundDecoy()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &ASoundDecoy::OnHit);


	ConstructorHelpers::FObjectFinder<USoundCue> temp(TEXT("/Script/Engine.SoundCue'/Game/ProjectAbyss/Blueprints/DeleteLater/HELP.HELP'"));
	

	DecoySoundTest = CreateDefaultSubobject<UAudioComponent>(TEXT("Decoy Audio"));
	DecoySoundTest->SetSound(temp.Object);
	DecoySoundTest->bOverrideAttenuation = true;
	FSoundAttenuationSettings tempatten;

	tempatten.AttenuationShape = EAttenuationShape::Sphere;
	tempatten.FalloffDistance = 4500.0f;
	tempatten.AttenuationShapeExtents = FVector(3000.0f, 0.0f, 0.0f);
	DecoySoundTest->AdjustAttenuation(tempatten);

	ItemName = "Sound Decoy";
	bIsPermanent = false;
	Cooldown = 60.0f;
	bIsConsumable = true;

	bCanThrow = true;
	bIsCharging = false;
	MinChargeValue = 1.0f;
	MaxChargeValue = 3.0f;
	ChargeValue = MinChargeValue;
	TrajectorySimTime = 3.0f;
	TrajectorySimFrequency = 30.0f;
	NumTragectoryMeshes = 50;


	DecoySoundTest->OnAudioFinished.AddUniqueDynamic(this, &ASoundDecoy::AudioFinished);

	ShopCost = 50;

	ItemDetails = FText::FromString("Throw to distract fish in the area for a short time.");
	
	

}

void ASoundDecoy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player") == false)
	{
		bIsThrown = false;
		
		
		//DecoySoundTest->Play();
		

		// TODO: Still have to rotate the object so it is flat on the impact surface.
		FVector hitNormal = Hit.Normal;
		FQuat hitRot = FQuat::MakeFromRotationVector(hitNormal);
		FVector upVector = GetActorUpVector();

		
	}
}

void ASoundDecoy::AudioFinished()
{
	
}

void ASoundDecoy::Noise()
{

	StoredPlayer->bSoundDecoyPlaying = true;
	MakeNoise(1.0f, (APawn*)this, GetActorLocation(), 4000.0f);
	TimeElapsed += 0.1f;
	if (TimeElapsed >= DecoySound->Duration - 3.0f)
	{
		StoredPlayer->bSoundDecoyPlaying = false;
		GetWorldTimerManager().ClearTimer(DecoyTimer);
		TimeElapsed = 0.0f;
	}
}

void ASoundDecoy::UsePrimary()
{
	Super::UsePrimary();

	Unequip();

	StartThrow();

	OnDropped();
}

void ASoundDecoy::ReleasePrimary()
{
	Super::ReleasePrimary();
}

void ASoundDecoy::UseSecondary()
{
	Super::UseSecondary();

	ResetTragectory();
}

void ASoundDecoy::ReleaseSecondary()
{
	Super::ReleaseSecondary();

	TurnOffTrajectory();
}

void ASoundDecoy::OnDropped()
{
	Super::OnDropped();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DecoySound, GetActorLocation(), 0.25f, 1.0f, 0.0f, DecoyAttenuation);
	GetWorldTimerManager().SetTimer(DecoyTimer, this, &ASoundDecoy::Noise, 0.1f, true, 0.0f);
}

void ASoundDecoy::BeginPlay()
{
    Super::BeginPlay();
    CreateObjectPool();
	StoredPlayer = Cast<ADiverCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ASoundDecoy::Interacted(ADiverCharacter* Player)
{
    Super::Interacted(Player);

	StoredPlayer = Player;
}

void ASoundDecoy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}
