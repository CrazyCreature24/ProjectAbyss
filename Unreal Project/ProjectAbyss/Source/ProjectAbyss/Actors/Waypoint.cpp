// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Waypoint.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "DiverCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WaypointWidget.h"
#include "Components/TextBlock.h"
#include "CompassIconWidget.h"
#include <DiverController.h>
#include "CompassWidget.h"
#include "Components/Overlay.h"
// Sets default values
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitboxComponent"));
	SetRootComponent(HitboxComponent);
	HitboxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HitboxComponent->SetGenerateOverlapEvents(true);	
	HitboxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::OnHitboxBeginOverlap);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(HitboxComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(500.0f, 500.0f));
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetVisibility(true);
	WidgetComponent->SetWidgetClass(WaypointWidgetClass);


	Tags.Add(TEXT("Waypoint"));
}

void AWaypoint::ToggleVisibility(bool bVisible)
{
	//Toggles the visibility of the waypoint
	bWaypointActive = bVisible;
	WidgetComponent->SetVisibility(bVisible);
}

void AWaypoint::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If the player overlaps, make the waypoint visible
	if (OtherActor->IsA(ADiverCharacter::StaticClass()))
	{
		ToggleVisibility(true);
	}
}

void AWaypoint::UpdateDistanceToPlayer()
{
	//Updates the distance to the player
	ADiverCharacter* DiverCharacter = Cast<ADiverCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (DiverCharacter)
	{
		DistanceToPlayer = FVector::Dist(DiverCharacter->GetActorLocation(), GetActorLocation());
		//Float to string
		Cast<UWaypointWidget>(WidgetComponent->GetWidget())->DistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), DistanceToPlayer/500) + "m"));
	}

}

void AWaypoint::UpdateData(bool bVisible, FString name, EWaypointType waypoint)
{
	//Updates the data of the waypoint
	ToggleVisibility(bVisible);
	WaypointName = name;
	WaypointType = waypoint;
	Cast<UWaypointWidget>(WidgetComponent->GetWidget())->SetWaypointImage(WaypointType);
}

void AWaypoint::Test()
{

	// Create the widget
	WaypointWidget = Cast<UWaypointWidget>(WidgetComponent->GetUserWidgetObject());

}


// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

	//Timer for test function
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWaypoint::Test, 1.0f, false, 1.0f);


	SetActorHiddenInGame(true);
}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the waypoint is visible, update the distance to the player
	if (bWaypointActive)
	{
		UpdateDistanceToPlayer();
	}

}

