// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Equipment/DeployableLight.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "InventoryComponent.h"


ADeployableLight::ADeployableLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Attachment"));
	LightComponent->SetupAttachment(RootComponent);
	SetLightVisibility(false);
	
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &ADeployableLight::OnHit);
	
	ItemName = "Deployable Light";
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


	bReplicates = true;
	SetReplicateMovement(true);

	ShopCost = 9999999;

	ItemName = "Deployable Light";

	ItemDetails = FText::FromString("Throwable light that lights up an area around it.");

	MeshComponent->CastShadow = false;
	LightComponent->SetRelativeLocation(FVector(20.0f, 0, 10.0f));

}

void ADeployableLight::BeginPlay()
{
	Super::BeginPlay();

	CreateObjectPool();

}

void ADeployableLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
}

// Primary - Throws the light (If not aiming primary already, then just place directly in front of the player)
void ADeployableLight::UsePrimary()
{
	// Left click Press
	Super::UsePrimary();

	Unequip();

	StartThrow();

	OnDropped();

	SetLightVisibility(true);

}

void ADeployableLight::ReleasePrimary()
{
	// Left click Release
	Super::ReleasePrimary();

	// Nothing yet


}

// Secondary (Hold down to aim) - Hold right click to charge up a "trajectory" that increases range the longer you hold it.
void ADeployableLight::UseSecondary()
{
	// Right click Press
	Super::UseSecondary();

	ResetTragectory();
	

}

// ReleaseSecondary - Stops the charge up and reset to default state
void ADeployableLight::ReleaseSecondary()
{
	// Right click Release
	Super::ReleaseSecondary();
	
	TurnOffTrajectory();
}

void ADeployableLight::OnDropped()
{
	Super::OnDropped();

}

void ADeployableLight::Interacted(ADiverCharacter* Player)
{
	Super::Interacted(Player);

}

void ADeployableLight::SetLightVisibility(bool bIsVisible)
{

	LightComponent->SetVisibility(bIsVisible);

}

void ADeployableLight::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, TEXT("Light On Hit registered"));

	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player") == false && bIsThrown == true)
	{
		bIsThrown = false;

		//APawn* player = Cast<A>(CurrentOwner);

		APawn* temp = NewObject<APawn>(this, "temp");
		temp->RegisterAllComponents();

		temp->SetActorLocation(GetActorLocation());

		MakeNoise(1.0f, temp, GetActorLocation(), 2000.0f);
		
	}
}

void ADeployableLight::Equip(AActor* NewOwner)
{
	Super::Equip(NewOwner);

	SetLightVisibility(false);
}
