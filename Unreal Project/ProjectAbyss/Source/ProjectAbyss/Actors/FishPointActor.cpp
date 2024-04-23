// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/FishPointActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AFishPointActor::AFishPointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	Sphere->SetCollisionProfileName("BlockAll");
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = Sphere;

	// Mesh Setup
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fish"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName("BlockAll");
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

}

// Called when the game starts or when spawned
void AFishPointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFishPointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

