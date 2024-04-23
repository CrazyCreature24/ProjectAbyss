// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/InteractableActor.h"
#include "Components/BoxComponent.h"
#include "DiverController.h"
#include "Components/WidgetComponent.h"
#include "DiverCharacter.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	OutlineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("OutlineMeshComponent");
	OutlineMeshComponent->SetupAttachment(RootComponent);
	OutlineMeshComponent->SetVisibility(false);

	HitboxComponent = CreateDefaultSubobject<UBoxComponent>("HitboxComponent");
	HitboxComponent->SetupAttachment(RootComponent);

	InteractHitboxComponent = CreateDefaultSubobject<UBoxComponent>("InteractHitboxComponent");
	InteractHitboxComponent->SetupAttachment(RootComponent);
	InteractHitboxComponent->ComponentTags = { "InteractHitbox" };
	InteractHitboxComponent->SetCollisionProfileName("BlockAll");

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteraxtWidget"));
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidget->SetDrawSize(FVector2D(60.0f, 60.0f));
	InteractWidget->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	InteractWidget->SetVisibility(false);

	// Add tag to interactable actor
	Tags.Add("Interactable");
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::Interacted(ADiverCharacter* Player)
{
	//On screen debug message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interacted with Interactable Actor"));
	FKey key = FKey("E");

	
}

