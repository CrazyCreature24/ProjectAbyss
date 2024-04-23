// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/LightSwitch.h"
#include "Boat.h"
#include "DiverCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ChildActorComponent.h"
#include "Components/PointLightComponent.h"

ALightSwitch::ALightSwitch()
{
	OffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OffMesh"));
	OffMesh->SetupAttachment(RootComponent);
}

void ALightSwitch::Interacted(ADiverCharacter* Player)
{
	AInteractableActor::Interacted(Player);

	//If the mesh is visible, turn it off and turn on the off mesh
	if (MeshComponent->IsVisible())
	{
		MeshComponent->SetVisibility(false);
		OffMesh->SetVisibility(true);
	}
	else
	{
		MeshComponent->SetVisibility(true);
		OffMesh->SetVisibility(false);
	}

	//Get all actors of class for boat
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), FoundActors);

	//If there is a boat
	if (FoundActors.Num() > 0)
	{
		ABoat* Boat = Cast<ABoat>(FoundActors[0]);

		//Get child of boat with the component tag name of "LightInside"
		TArray<UChildActorComponent*> FoundChildActors;
		Boat->GetComponents(FoundChildActors);

		for (UChildActorComponent* Child : FoundChildActors)
		{
			if (Child->ComponentHasTag("LightInside"))
			{
				AActor* Light = Child->GetChildActor();
				//Gets components of the Light
				TArray<UPointLightComponent*> FoundLights;
				Light->GetComponents(FoundLights);

				for (UPointLightComponent* LightComponent : FoundLights)
				{
					if (LightComponent->IsVisible())
					{
						LightComponent->SetVisibility(false);
					}
					else
					{
						LightComponent->SetVisibility(true);
					}
				}

				
			}
		}

	}
}

void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();
}
