// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/BigMenuEyeball.h"
#include "Kismet/GameplayStatics.h"
#include "DiverCharacter.h"

// Sets default values
ABigMenuEyeball::ABigMenuEyeball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //Creates meshes
    FishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    FishMesh->SetupAttachment(RootComponent);

    PupilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pupil"));
    PupilMesh->SetupAttachment(FishMesh);

}

// Called when the game starts or when spawned
void ABigMenuEyeball::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABigMenuEyeball::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector mouseLocation;
    FVector mouseDirection;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

    FVector actorLocation = GetActorLocation();
    FVector endLocation = FMath::LinePlaneIntersection(mouseLocation, mouseLocation + mouseDirection, actorLocation, FVector(1.0f, 0.0f, 0.0f));

    float distanceToMouse = FVector::Dist(actorLocation, endLocation);

    float maxRadius = 220.0f;

    if (distanceToMouse > maxRadius)
    {
        FVector clampedLocation = actorLocation + (endLocation - actorLocation).GetSafeNormal() * maxRadius;

        // Use interpolation for smooth movement
        FVector lerpedLocation = FMath::VInterpTo(PupilMesh->GetComponentLocation(), FVector(75.0f, clampedLocation.Y, clampedLocation.Z), DeltaTime, 2.0f);

        // Move the pupil mesh location to the interpolated location
        PupilMesh->SetWorldLocation(lerpedLocation);
    }
    else
    {
        // Use interpolation for smooth movement
        FVector lerpedLocation = FMath::VInterpTo(PupilMesh->GetComponentLocation(), FVector(75.0f, endLocation.Y, endLocation.Z), DeltaTime, 2.0f);

        // Move the pupil mesh location to the interpolated location
        PupilMesh->SetWorldLocation(lerpedLocation);
    }
}
