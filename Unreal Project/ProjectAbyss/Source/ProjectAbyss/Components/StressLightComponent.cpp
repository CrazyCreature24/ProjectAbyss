// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/StressLightComponent.h"
#include "StressComponent.h"
#include "Components/PointLightComponent.h" 
#include "Components/SphereComponent.h" 
#include "DiverCharacter.h"

// Sets default values for this component's properties
UStressLightComponent::UStressLightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Inside Volume");
	Sphere->SetCollisionProfileName("NoCollision");
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &UStressLightComponent::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &UStressLightComponent::OnOverlapEnd);
	Sphere->SetupAttachment(GetAttachmentRoot());
}


// Called when the game starts
void UStressLightComponent::BeginPlay()
{
	Super::BeginPlay();
	
	myLight = GetOwner()->GetComponentByClass<UPointLightComponent>();
	OGIntensity = myLight->Intensity;
}


// Called every frame
void UStressLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bShouldTick || !bIsStillBright)
		return;

	float maxDist = Sphere->GetScaledSphereRadius();
	for (UStressComponent* a : myPlayers)
	{
		FHitResult Hit;

		FVector Start = GetOwner()->GetActorLocation();
		FVector End = a->GetOwner()->GetActorLocation();
		FCollisionQueryParams TraceParams;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 1.f);

		if ((Hit.GetActor() == a->GetOwner() || Hit.GetActor() == nullptr) && myLight)
		{
			if (a->SetStressDecrease((StressDecreasedPersecond * DeltaTime) / 1 - (Hit.Distance / maxDist))) { // change hard coded values

				myLight->SetIntensity(myLight->Intensity - (OGIntensity * PercentageLightDecrease / 100)  * DeltaTime); // TODO Change to percentage
				if (myLight->Intensity <= 0)
				{
					myLight->SetIntensity(0);
				}

				if (myLight->Intensity < 100)
				{
					bIsStillBright = false;
					for (UStressComponent* b : myPlayers)
					{
						b->SetStressDecrease(-1);
					}
					break;
				}
			}
		}
		else
			a->SetStressDecrease(-1);
	}

}

void UStressLightComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UStressComponent* PlayerStress = OtherActor->GetComponentByClass<UStressComponent>();

	bShouldTick = true;

	if (PlayerStress)
	{
		myPlayers.AddUnique(PlayerStress);
	}
}

void UStressLightComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UStressComponent* PlayerStress = OtherActor->GetComponentByClass<UStressComponent>();

	if (PlayerStress)
	{
		PlayerStress->SetStressDecrease(-1);
		myPlayers.Remove(PlayerStress);

		bShouldTick = myPlayers.IsEmpty() ? false : true;
	}
}

