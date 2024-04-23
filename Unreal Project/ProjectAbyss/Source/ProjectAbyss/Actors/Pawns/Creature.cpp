// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Pawns/Creature.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/InFocusWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/MissionObjectiveIDComponent.h"
#include "ScannedWidget.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DiverCharacter.h"



// Sets default values
ACreature::ACreature()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetBoxExtent(FVector(25, 25, 25));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionProfileName("BlockAll");
	CollisionBox->SetSimulatePhysics(false);

	// Mesh Setup
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fish Mesh"));
	Mesh->SetupAttachment(CollisionBox);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>BubbleSystem(TEXT("'/Game/ProjectAbyss/Art/Niagara/Systems/NS_Smallbubbles.NS_Smallbubbles'"));

	// Make niagara component
	BubblesNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	BubblesNiagaraComponent->SetupAttachment(Mesh);
	BubblesNiagaraComponent->SetAutoActivate(true);
	BubblesNiagaraComponent->SetAsset(BubbleSystem.Object);


	//static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));
	//UStaticMesh* Asset = MeshAsset.Object;
	//Mesh->SetStaticMesh(Asset);
	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));


	// Set focus class to folder path
	static ConstructorHelpers::FClassFinder<UInFocusWidget> FocusWidgetClassFinder(TEXT("/Game/ProjectAbyss/Blueprints/Widgets/WBP_CreatureInFocus"));
	if (FocusWidgetClassFinder.Succeeded())
	{
		FocusWidgetClass = FocusWidgetClassFinder.Class;
	}


	// Setup focus widget component
	FocusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Focus Widget"));
	FocusWidgetComponent->SetupAttachment(CollisionBox);
	FocusWidgetComponent->SetWidgetClass(FocusWidgetClass);
	FocusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FocusWidgetComponent->SetDrawSize(FVector2D(400, 400));
	FocusWidgetComponent->SetRelativeLocation(FVector(-20, -10, -10));
	FocusWidgetComponent->SetVisibility(false);

	//Setup Scanned Widget
	ScannedWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Scanned Widget"));
	ScannedWidgetComponent->SetupAttachment(CollisionBox);
	ScannedWidgetComponent->SetWidgetClass(ScannedWidgetClass);
	ScannedWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	ScannedWidgetComponent->SetDrawSize(FVector2D(400, 400));
	ScannedWidgetComponent->SetRelativeLocation(FVector(0, 0, -20));
	ScannedWidgetComponent->SetVisibility(false);


	//Constructor helper load for textures
	static ConstructorHelpers::FObjectFinder<UTexture2D>RedCameraBoxTextureAsset(TEXT("'/Game/ProjectAbyss/Art/Textures/red-box.red-box'"));
	RedCameraBoxTexture = RedCameraBoxTextureAsset.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D>BlueCameraBoxTextureAsset(TEXT("'/Game/ProjectAbyss/Art/Textures/blue-box.blue-box'"));
	BlueCameraBoxTexture = BlueCameraBoxTextureAsset.Object;


	//QuestIDComponent Setup (Nico Mod)
	ObjectiveIDsComponent = CreateDefaultSubobject<UMissionObjectiveIDComponent>(TEXT("Objective IDs"));

	//QuestIDs->ObjectiveIDs.Add(ScientificName);

	Tags.Add("Creature");
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();

	ScannedWidgetComponent->SetWidgetClass(ScannedWidgetClass);

	if (FocusWidgetComponent)
	{
		if (FocusWidgetClass)
		Cast<UInFocusWidget>(FocusWidgetComponent->GetWidget())->SetCreatureName(this);
	}

	if (ScannedWidgetComponent)
	{
		if (ScannedWidgetClass)
		{
			if (ScannedWidgetComponent->GetWidget())
			{
				Cast<UScannedWidget>(ScannedWidgetComponent->GetWidget())->SetCreature(this);
			}
		}
    }

	//Get the diver using found actor
	DiverRef = Cast<ADiverCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//MoveIgnoreActorAdd();
	
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (DiverRef) 
	{
		// Calculate the direction vector from the creature to the player
		FVector CreatureToPlayerDirection = DiverRef->GetActorLocation() - GetActorLocation();

		// Calculate the rotation needed to make the widget face the player
		FRotator NewWidgetRotation = CreatureToPlayerDirection.ToOrientationRotator();

		// Apply the rotation to the widget component
		if (ScannedWidgetComponent)
		{
			ScannedWidgetComponent->SetWorldRotation(NewWidgetRotation);
		}
	}

}

// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


//#if WITH_EDITOR
//void ACreature::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//
//	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACreature, bHasBubbles))
//	{
//		SetHasBubbles(bHasBubbles);
//	}
//
//
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//}
//#endif


void ACreature::SetHasBubbles(bool HasBubbles)
{
	if (BubblesNiagaraComponent)
	{
		BubblesNiagaraComponent->SetAutoActivate(HasBubbles);
	}
}

bool ACreature::GetIsInFocus()
{
	return bIsInFocus;
}

void ACreature::SetIsInFocus(bool bInFocus)
{
	bIsInFocus = bInFocus;
}

void ACreature::SetCaptured()
{
	bFirstTimeBonusAvailable = false;
	
	if (FocusWidgetComponent)
	Cast<UInFocusWidget>(FocusWidgetComponent->GetWidget())->SetCreatureName(this);

	if (ScannedWidgetComponent)
	Cast<UScannedWidget>(ScannedWidgetComponent->GetWidget())->SetCreature(this);
}

void ACreature::SetPictureCooldown()
{
	if (bIsPictureCooldown == false)
	{
		bIsPictureCooldown = true;
	}
	else	
	{
		bIsPictureCooldown = false;
	}
}

bool ACreature::GetPictureCooldown()
{
	return bIsPictureCooldown;
}

void ACreature::ShowFocusWidget()
{

	if (FocusWidgetComponent)
	{
		FocusWidgetComponent->SetVisibility(true);
	}

}

void ACreature::HideFocusWidget()
{
	if (FocusWidgetComponent)
	{
		FocusWidgetComponent->SetVisibility(false);
	}
}

void ACreature::ShowScannedWidget()
{
	if (ScannedWidgetComponent)
	{
		if (ScannedWidgetComponent->GetWidget())
		{
			ScannedWidgetComponent->SetVisibility(true);
			Cast<UScannedWidget>(ScannedWidgetComponent->GetWidget())->SetCreature(this);
			Cast<UScannedWidget>(ScannedWidgetComponent->GetWidget())->StartAnimation();
		}
    }

}

void ACreature::HideScannedWidget()
{
	if (ScannedWidgetComponent)
	{
        ScannedWidgetComponent->SetVisibility(false);
    }
}

void ACreature::PlayScannedAnimation()
{
	if (ScannedWidgetComponent)
	{
		Cast<UScannedWidget>(ScannedWidgetComponent->GetWidget())->PlayAnimation(Cast<UScannedWidget>(ScannedWidgetComponent->GetWidget())->ScannedAnim, 0.0f, 1);
    }
}

void ACreature::StartCooldownTimer()
{
	bIsPictureCooldown = true;

	//Sets timer for picture cooldown
	GetWorldTimerManager().SetTimer(PictureCooldownTimerHandle, this, &ACreature::SetPictureCooldown, PictureCooldown, false);

}


/*
ChangeLog:
---------------------------- -
Date : November 11
Name : Jarod Beach
Summary : Class Created and Moved code into
---------------------------- -
*/