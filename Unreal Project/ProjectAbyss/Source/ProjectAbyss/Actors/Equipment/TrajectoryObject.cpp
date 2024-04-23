// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Equipment/TrajectoryObject.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATrajectoryObject::ATrajectoryObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CastShadow = false;
}

// Called when the game starts or when spawned
void ATrajectoryObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrajectoryObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

