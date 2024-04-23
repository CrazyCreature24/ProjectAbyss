// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/PhotographyCamera.h"
#include "PhotographyComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CameraViewfinder.h"
#include "Fish.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include <DiverController.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/SpringArmComponent.h"
#include "Components/MissionObjectiveIDComponent.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Components/PostProcessComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APhotographyCamera::APhotographyCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractHitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Box collision
	FocusHitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FocusHitboxComponent"));
	FocusHitboxComponent->SetBoxExtent(FVector(1000.f, 150.f, 120.f));
	FocusHitboxComponent->SetRelativeLocation(FVector(0.0f, 0.f, 0.f));
	FocusHitboxComponent->SetupAttachment(MeshComponent);
	// Default no collision for focus hitbox
	FocusHitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Photography component
	PhotographyCameraComponent = CreateDefaultSubobject<UPhotographyComponent>(TEXT("PhotographyComponent"));

	// Spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(MeshComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, 0.f, 180.f));
	SpringArmComponent->TargetArmLength = 50.f;
	SpringArmComponent->bDoCollisionTest = true;

	// Camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(60.f);


	// scene component
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneComponent"));
	SceneCaptureComponent->SetupAttachment(CameraComponent);
	SceneCaptureComponent->FOVAngle = 30.f;
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;


	// Bind overlap events
	FocusHitboxComponent->OnComponentBeginOverlap.AddDynamic(this, &APhotographyCamera::OnFocusHitboxBeginOverlap);
	FocusHitboxComponent->OnComponentEndOverlap.AddDynamic(this, &APhotographyCamera::OnFocusHitboxEndOverlap);

	//// Post process component
	PostProcessNightVision = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessNightVision->SetupAttachment(CameraComponent);
	PostProcessNightVision->bUnbound = true;


	PostProcessThermalVision = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent1"));
	PostProcessThermalVision->SetupAttachment(CameraComponent);
	PostProcessThermalVision->bUnbound = true;




	// Set camera viewfinder class to folder path
	static ConstructorHelpers::FClassFinder<UCameraViewfinder> CameraViewfinderClassFinder(TEXT("/Game/ProjectAbyss/Blueprints/Widgets/WBP_ViewfinderCamera"));
	if (CameraViewfinderClassFinder.Succeeded())
	{
		ViewfinderClass = CameraViewfinderClassFinder.Class;
	}

	ItemName = "Photography Camera";
	ShopCost = 9999999;

	ItemDetails = FText::FromString("This is a camera");


	Tags.Add("Photography Camera");


}

// Called when the game starts or when spawned
void APhotographyCamera::BeginPlay()
{
	Super::BeginPlay();

	SceneCaptureComponent->bCaptureOnMovement = false;

	DefaultRotation = GetActorRotation();

	DefaultCameraLocation = FocusHitboxComponent->GetRelativeLocation();

	ADiverCharacter* Diver = Cast<ADiverCharacter>(CurrentOwner);
	if (Diver)
	{
		DiverRef = Diver;
	}


	PostProcessNightVision->bEnabled = false;
	PostProcessThermalVision->bEnabled = false;

}

void APhotographyCamera::OnFocusHitboxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If other actor has tag fish
	if (OtherActor->ActorHasTag("Fish"))
	{
		if (OverlappedComp->IsA<UBoxComponent>())
		{
			//Show debug saying focus hitbox overlapped
			GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red, TEXT("Focus hitbox overlapped"));

			//Cast class to fish
			ACreature* Fish = Cast<ACreature>(OtherActor);
			if (Fish)
			{
				//If fish is not already in focus
				if (Fish->GetIsInFocus() == false)
				{
					//Gets distance from fish to camera
					float DistanceToFish = GetDistanceTo(OtherActor);

					//Add fish to focus array
					PhotographyCameraComponent->AddFishInFocus(Fish, DistanceToFish);

				}
			}
		}

	}
}

void APhotographyCamera::OnFocusHitboxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	//If other actor has tag fish
	if (OtherActor->ActorHasTag("Fish"))
	{
		if (OverlappedComp->IsA<UBoxComponent>())
		{
			//Cast class to fish
			ACreature* Fish = Cast<AFish>(OtherActor);
			if (Fish)
			{
				//Remove actor from current focus fish array
				PhotographyCameraComponent->RemoveFishInFocus(Cast<class AFish>(OtherActor));

				//If fish is in focus
				if (Fish->GetIsInFocus() == true)
				{
					//Set fish to not be in focus
					Fish->SetIsInFocus(false);
					//Hide screenspace widget
					Fish->HideFocusWidget();
				}

			}
		}

	}
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red, TEXT("Focus hitbox finished overlapping"));

}

void APhotographyCamera::SetFocusHitboxCollision(bool bIsEnabled)
{
	if (bIsEnabled == true)
	{
		FocusHitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Debug message
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Focus hitbox enabled"));
	}
	else
	{
		FocusHitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Debug message
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Focus hitbox disabled"));
	}
}

void APhotographyCamera::SetAimingMode(bool bIsAiming)
{
	bIsAimingCam = bIsAiming;
	//Hides player mesh
	if (Cast<ADiverCharacter>(CurrentOwner))
	{
		Cast<ADiverCharacter>(CurrentOwner)->GetMesh()->SetVisibility(!bIsAiming);
	}
}

bool APhotographyCamera::GetAimingMode()
{
	return bIsAimingCam;
}

void APhotographyCamera::SetCanTakePicture(bool bCanTake)
{
	bCanTakePicture = bCanTake;
}

bool APhotographyCamera::GetCanTakePicture()
{
	return bCanTakePicture;
}

ECameraMode APhotographyCamera::GetCameraMode()
{
	return CameraMode;
}

void APhotographyCamera::SetCameraMode(ECameraMode mode)
{
	CameraMode = mode;
	//Switch post process to the current mode


	switch (CameraMode)
	{
	case ECameraMode::Normal:
		//Set post process to normal
		PostProcessNightVision->bEnabled = false;
		PostProcessThermalVision->bEnabled = false;
		break;

	case ECameraMode::NightVision:
		//Set post process to night vision
		PostProcessNightVision->bEnabled = true;
		PostProcessThermalVision->bEnabled = false;
		break;

	case ECameraMode::ThermalVision:
		//Set post process to thermal vision
		PostProcessNightVision->bEnabled = false;
		PostProcessThermalVision->bEnabled = true;
		break;

	default:
		break;
	}

}

void APhotographyCamera::ChangeCameraMode()
{
	// Get the current camera mode
	ECameraMode CurrentMode = GetCameraMode();

	// Calculate the next camera mode
	ECameraMode NextMode = static_cast<ECameraMode>((static_cast<int32>(CurrentMode) + 1) % static_cast<int32>(ECameraMode::Count));

	// Set the next camera mode
	SetCameraMode(NextMode);
}

void APhotographyCamera::UpdateFishInFocus()
{
	TMap<ACreature*, float>& FishInFocusMap = PhotographyCameraComponent->FishInFocus;
	// Loops through all keys in the FishInFocus map
	for (auto& FishEntry : FishInFocusMap)
	{
		// Finds the fish closest to the camera and sets it to be in focus
		ACreature* CurrentFish = FishEntry.Key;
		float CurrentDistance = FishEntry.Value;

		// Check if the current fish is closer than the currently focused fish
		if (CurrentFish == PhotographyCameraComponent->GetClosestFishInFocus())
		{
			// Set the current fish to be in focus
			CurrentFish->SetIsInFocus(true);
			// Trigger screenspace widget to show fish focus
			CurrentFish->ShowFocusWidget();
		}
		else
		{
			// Set the current fish to not be in focus
			CurrentFish->SetIsInFocus(false);
			// Hide screenspace widget
			CurrentFish->HideFocusWidget();
		}
	}
}



void APhotographyCamera::ResetPictureCooldown()
{
	bCanTakePicture = true;
}

void APhotographyCamera::UsePrimary()
{
	AEquipmentBase::UsePrimary();

	// Check if the picture is on cooldown
	if (bCanTakePicture == false)
	{
		return;
	}

	if (GetAimingMode() == false)
	{
		return;
	}

	if (PhotographyCameraComponent->CurrentFilm <= 0)
	{
		// DISPLAY NO FILM MESSAGE ON VIEWFINDER
		Viewfinder->PlayOutOfFilmAnimation();
		return;
	}

	PhotographyCameraComponent->TakePicture(this);

	if (Viewfinder)
	{
		Viewfinder->ActivateShutter();
		Viewfinder->SetPictureCounter(PhotographyCameraComponent->CurrentFilm);
	}


	//Play camera sound effect
	UGameplayStatics::PlaySoundAtLocation(this, PhotoTakenSound, GetActorLocation());
	// Set the picture to be on cooldown
	bCanTakePicture = false;
	// Set the timer to reset the cooldown
	GetWorldTimerManager().SetTimer(PictureTimerHandleCooldown, this, &APhotographyCamera::ResetPictureCooldown, PictureCooldown, false);


	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();//for testing
	MissionLog->CallObjectiveID("TestPic");

	ACreature* FishInFocus = PhotographyCameraComponent->GetClosestFishInFocus();

	if (FishInFocus != nullptr)
	{
		FishInFocus->ObjectiveIDsComponent->ObjectiveIDs.Contains("RoboSharkPic");
		MissionLog->CallObjectiveID("RoboSharkPic"); //hack fix for tutorial
		//FishInFocus->QuestIDs->BroadcastObjectiveIDs(); 
	}


}

void APhotographyCamera::UseSecondary()
{
	AEquipmentBase::UseSecondary();
	// Set camera to be the view target (THIS ONLY IS CALLED ONCE)
	ADiverController* DiverController = Cast<ADiverController>(Cast<ADiverCharacter>(CurrentOwner)->GetController());
	if (bIsAimingCam == false)
	{
		// Set aiming mode to true
		SetAimingMode(true);

		// Sets the focus hitbox to be enabled
		SetFocusHitboxCollision(true);

		// Set the camera to be the view target
		if (DiverController)
		{
			DiverController->SetViewTargetWithBlend(this, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		}

		// Create viewport widget
		if (ViewfinderClass)
		{
			UUserWidget* ViewfinderWidget = CreateWidget(GetWorld(), ViewfinderClass);
			if (ViewfinderWidget)
			{
				ViewfinderWidget->AddToViewport();
				Viewfinder = Cast<UCameraViewfinder>(ViewfinderWidget);
				Viewfinder->SetPictureCounter(PhotographyCameraComponent->CurrentFilm);
			}
		}
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	DiverController->GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Set camera rotation to player rotation
	SetActorRotation(CameraRotation);

	UpdateFishInFocus();

}

void APhotographyCamera::ReleaseSecondary()
{
	AEquipmentBase::ReleaseSecondary();
	SetAimingMode(false);

	// Sets the focus hitbox to be enabled
	SetFocusHitboxCollision(false);

	// Set to default rotation
	SetActorRotation(GetOwner()->GetActorRotation());

	if (PostProcessNightVision && PostProcessThermalVision)
	{
		PostProcessNightVision->bEnabled = false;
		PostProcessThermalVision->bEnabled = false;
	}

	CameraMode = ECameraMode::Normal;

	SpringArmComponent->TargetArmLength = 50.f;
	FocusHitboxComponent->SetRelativeLocation(DefaultCameraLocation);

	// Remove viewfinder widget from viewport
	if (Viewfinder)
	{
		Viewfinder->RemoveFromParent();
	}

	// Cast to diver controller

	if (CurrentOwner)
	{
		ADiverController* DiverController = Cast<ADiverController>(Cast<ADiverCharacter>(CurrentOwner)->GetController());
		if (DiverController) {
			DiverController->SetViewTargetWithBlend(DiverController->GetPawn(), 0.0f);
		}
	}

}

void APhotographyCamera::Equip(AActor* NewOwner)
{
	Super::Equip(NewOwner);
	SetActorRelativeLocation(FVector(0.f, 0.f, 60.f));
	this->SetActorEnableCollision(true);
	//Cast new owner to diver character
	if (NewOwner->IsA<ADiverCharacter>())
	{
		ADiverCharacter* Player = Cast<ADiverCharacter>(NewOwner);
		Player->CameraActor = this;
	}

}

// Called every frame
void APhotographyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PhotographyCameraComponent->UpdateFishInFocusDistances();

}


// DEPRECATED FOR REFERENCE
//void APhotographyCamera::AimCamera(ADiverController* controller)
//{
//	//// Set camera to be the view target (THIS ONLY IS CALLED ONCE)
//	//if (bIsAimingCam == false)
//	//{
//	//	DiverControllerRef = controller;
//	//	// Set aiming mode to true
//	//	SetAimingMode(true);
//
//	//	// Sets the focus hitbox to be enabled
//	//	SetFocusHitboxCollision(true);
//
//	//	// Set camera to be the view target
//	//	controller->SetViewTargetWithBlend(this, 0.5f);
//
//	//	// Create viewport widget
//	//	if (ViewfinderClass)
//	//	{
//	//		UUserWidget* ViewfinderWidget = CreateWidget(GetWorld(), ViewfinderClass);
//	//		if (ViewfinderWidget)
//	//		{
//	//			ViewfinderWidget->AddToViewport();
//	//			Viewfinder = Cast<UCameraViewfinder>(ViewfinderWidget);
//	//		}
//	//	}
//	//}
//
//	//FVector CameraLocation;
//	//FRotator CameraRotation;
//	//DiverControllerRef->GetActorEyesViewPoint(CameraLocation, CameraRotation);
//
//	//// Set camera rotation to player rotation
//	//SetActorRotation(CameraRotation);
//
//	//UpdateFishInFocus();
//
//}
//
//void APhotographyCamera::StopAimingCamera(ADiverController* controller)
//{
//	//SetAimingMode(false);
//
//	//// Sets the focus hitbox to be enabled
//	//SetFocusHitboxCollision(false);
//
//	//// Set to default rotation
//	//SetActorRotation(GetOwner()->GetActorRotation());
//
//	//PostProcessNightVision->bEnabled = false;
//	//PostProcessThermalVision->bEnabled = false;
//	//CameraMode = ECameraMode::Normal;
//
//	//SpringArmComponent->TargetArmLength = 200.f;
//	//FocusHitboxComponent->SetRelativeLocation(DefaultCameraLocation);
//
//	//// Remove viewfinder widget from viewport
//	//Viewfinder->RemoveFromParent();
//}
//
//void APhotographyCamera::TakePicture()
//{
//	//// Check if the picture is on cooldown
//	//if (bCanTakePicture == false)
//	//{
//	//	return;
//	//}
//
//	//PhotographyCameraComponent->TakePicture(this);
//
//	//if (PhotographyCameraComponent->GetClosestFishInFocus())
//	//{
//	//	if (PhotographyCameraComponent->GetClosestFishInFocus()->ActorHasTag("TutorialFish1"))
//	//	{
//	//		QuestComplete.Broadcast();
//	//		//Trigger delegate for quest 1
//	//	}
//	//	if (PhotographyCameraComponent->GetClosestFishInFocus()->ActorHasTag("TutorialFish2"))
//	//	{
//	//		//Trigger delegate for quest 2
//	//	}
//	//	if (PhotographyCameraComponent->GetClosestFishInFocus()->ActorHasTag("TutorialFish3"))
//	//	{
//	//		//Trigger delegate for quest 3	
//	//	}
//	//}
//
//
//	////Play camera sound effect
//	//UGameplayStatics::PlaySoundAtLocation(this, PhotoTakenSound, GetActorLocation());
//	//// Set the picture to be on cooldown
//	//bCanTakePicture = false;
//	//// Set the timer to reset the cooldown
//	//GetWorldTimerManager().SetTimer(PictureTimerHandleCooldown, this, &APhotographyCamera::ResetPictureCooldown, PictureCooldown, false);
//
//	//UQuestLogSubsystem* QuestLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UQuestLogSubsystem>();
//
//
//	//QuestLog->CallObjectiveID("OTestID2");
//	////Get Fish in focus and call QuestIDComponent Function (Nico Mod)
//	//AFish_Ai* FishInFocus = PhotographyCameraComponent->GetClosestFishInFocus();
//	//if(FishInFocus)
//	//FishInFocus->QuestIDs->BroadcastQuestIDs();
//
//}