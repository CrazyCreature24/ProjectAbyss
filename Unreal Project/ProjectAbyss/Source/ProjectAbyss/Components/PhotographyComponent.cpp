// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/PhotographyComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Fish.h"
#include "Creature.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "PhotographyCamera.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include <PhotographyCollectionSubsystem.h>
#include "Kismet/KismetSystemLibrary.h"
#include "HAL/FileManager.h"
#include "HAL/Platform.h"
#include "SaveLoadSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectAbyssGameInstance.h"

// Sets default values for this component's properties
UPhotographyComponent::UPhotographyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CameraPhotosFilePath = FString(UKismetSystemLibrary::GetProjectSavedDirectory() + FString("CameraPhotos"));

}


// Called when the game starts
void UPhotographyComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//test
	IFileManager& FileManager = IFileManager::Get();

	TArray<FString> Files;


	//FileManager.FindFiles(Files, FString(UKismetSystemLibrary::GetProjectSavedDirectory() + FString("\\CameraPhotos")).GetCharArray().GetData(), NULL);
	FileManager.FindFiles(Files,CameraPhotosFilePath.GetCharArray().GetData(), NULL);
	FString F = "la";


	TCHAR* gf = F.GetCharArray().GetData();
	//test

	USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();

	SaveLoad->LoadCamera();

}


// Called every frame
void UPhotographyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPhotographyComponent::TakePicture(APhotographyCamera* camera)
{
	
	// Store picture in gallery at first slot
	if (CurrentFilm > 0)
	{

		// Create a temporary render target with good image quality
		UTextureRenderTarget2D* TempRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024,ETextureRenderTargetFormat::RTF_RGBA8); // RTF_RGBA8 is PNG format		//Got set to 256 x 256 to help with crashing. // Testing 1024
		TempRenderTarget->TargetGamma = 2.0f;
		// Set your Scene Capture to “Final Color (LDR) in RGBE
		//TempRenderTarget->CompressionSettings = TC_HDR;
		TempRenderTarget->CompressionSettings = TC_Default;
		TempRenderTarget->SRGB = true;
		TempRenderTarget->bAutoGenerateMips = false;
		TempRenderTarget->UpdateResourceImmediate(true);

		camera->SceneCaptureComponent->TextureTarget = TempRenderTarget;	
		// Capture the scene
		camera->SceneCaptureComponent->CaptureScene();

		// Nico Saving Photos
		//UKismetSystemLibrary::GetProjectSavedDirectory();

		FString temp1 = "PhotoSaveTest";
		//FString temp2 = FString::FromInt(MaxFilm - CurrentFilm);
		int& TotalPhotosTaken = Cast<UProjectAbyssGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->TotalPhotosTaken;

		FString temp2 = FString::FromInt(TotalPhotosTaken);

		if (temp2.Len() < 4)
		{
			while (temp2.Len() < 4)
			{
				temp2 = "0" + temp2;
			}
		}

		FString temp3 = ".png";

		FString Filename = temp1 + temp2 + temp3;

		//TotalPhotosTaken++;
		
		//UKismetRenderingLibrary::ExportRenderTarget(GetWorld(), camera->SceneCaptureComponent->TextureTarget, UKismetSystemLibrary::GetProjectSavedDirectory(), Filename);
		//

		//// Store the captured texture in the array
		//if (PhotoGallery.IsValidIndex(MaxFilm-CurrentFilm))
		//PhotoGallery.Add(camera->SceneCaptureComponent->TextureTarget);
	

		UKismetRenderingLibrary::ExportRenderTarget(GetWorld(),TempRenderTarget, UKismetSystemLibrary::GetProjectSavedDirectory() + FString("\\CameraPhotos"), Filename); //test
	

		// If theres a fish currently in focus, add it to the captured fish array with its associated image
		//if (GetClosestFishInFocus() != nullptr)
		//{
		//	//CapturedFish.Add(GetClosestFishInFocus(), PhotoGallery[MaxFilm - CurrentFilm]);
		//	if (GetClosestFishInFocus()->bIsPictureCooldown == false)
		//	GetClosestFishInFocus()->StartCooldownTimer();
		//}

		//New System
		if (GetClosestFishInFocus() != nullptr)
		{
			FishCapturedNico.Add(GetClosestFishInFocus()->GetClass());
			if (GetClosestFishInFocus()->bIsPictureCooldown == false)
			GetClosestFishInFocus()->StartCooldownTimer();

		/*	ACreature* what = Cast<ACreature>(FishCapturedNico[0]->GetDefaultObject());

			if (what != nullptr)
			{
				int i = 1 + 2;
			}*/
		}

		//FField* something = GetClosestFishInFocus()->StaticClass();

		// Decrease film count
		CurrentFilm--; 

		TotalPhotosTaken++;
	}
	else
	{
		//Debug msg Out of film for the expedition
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Out of film for the expedition. Please reload it in your computer."));
	}

}

void UPhotographyComponent::ReloadCamera()
{
	CurrentFilm = MaxFilm;
}

void UPhotographyComponent::ResetCamera()
{
	CurrentFilm = MaxFilm;
	PhotoGallery.Empty();
	CapturedFish.Empty();
	PhotoGallery.Empty();
	FishCapturedNico.Empty();
}


void UPhotographyComponent::AddFishInFocus(class ACreature* Fish, float DistanceToPlayer)
{
	FishInFocus.Add(Fish, DistanceToPlayer);
}

TMap<ACreature*, float> UPhotographyComponent::GetFishInFocus()
{
	return FishInFocus;
}

void UPhotographyComponent::RemoveFishInFocus(ACreature* Fish)
{
	FishInFocus.Remove(Fish);
}

void UPhotographyComponent::UpdateFishInFocusDistances()
{
// Iterate through the FishInFocus TMap
	for (auto& FishEntry : FishInFocus)
	{
		ACreature* Fish = FishEntry.Key;
		if (Fish)
		{
			FVector CameraLocation = GetOwner()->GetActorLocation(); // Change this to your camera's location retrieval method
			FVector FishLocation = Fish->GetActorLocation();

			float Distance = FVector::Dist(CameraLocation, FishLocation);

			// Update the distance value in the map
			FishInFocus[Fish] = Distance;
		}
	}
}

ACreature* UPhotographyComponent::GetClosestFishInFocus()
{
	ACreature* ClosestFish = nullptr;
	float ClosestDistance = MAX_FLT;

	// Iterate through the FishInFocus TMap
	for (const auto& FishEntry : FishInFocus)
	{
		//If fish entry is empty

		ACreature* CurrentFish = FishEntry.Key;
		float CurrentDistance = FishEntry.Value;

		// Check if the current fish is closer than the current closest fish
		if (CurrentDistance < ClosestDistance)
		{
			ClosestFish = CurrentFish;
			ClosestDistance = CurrentDistance;
		}
	}

	return ClosestFish;
}


