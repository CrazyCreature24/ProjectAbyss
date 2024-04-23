// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhotographyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UPhotographyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhotographyComponent();

	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/// <summary>
	/// Takes a picture with the camera, if the camera has film left. Uses scene capture component from passed in camera to capture scene. Fish in focus are added to an array, and fish captured are stored in an array.
	/// </summary>
	/// <param name="camera"> The players camera who just took the photograph.</param>
	UFUNCTION()
      void TakePicture(APhotographyCamera* camera);

	/// <summary>
	/// Reloads the camera film to max.
	/// </summary>
	UFUNCTION()
      void ReloadCamera();

	/// <summary>
	/// Resets all of the cameras variables to default. This is used when the player dies.
	/// </summary>
	UFUNCTION()
      void ResetCamera();

	/// <summary>
	/// Called by photography camera to add fish in focus to the map. The map is used to keep track of the fish in focus and their distance to the player.
	/// </summary>
	/// <param name="Fish"> The fish to be added to the map </param>
	/// <param name="DistanceToPlayer"> The distance of the fish to the player to be added to the map </param>
	UFUNCTION()
	  void AddFishInFocus(class ACreature* Fish, float DistanceToPlayer);

	/// <summary>
	/// Gets the fish in focus map.
	/// </summary>
	/// <returns> Returns a single fish in focus with its distance </returns>
	UFUNCTION()
	  TMap<ACreature*, float> GetFishInFocus();

	/// <summary>
	/// Removes a specific fish in focus using the index of the fish in focus map.
	/// </summary>
	/// <param name="Fish"></param>
	UFUNCTION()
	  void RemoveFishInFocus(class ACreature* Fish);

	/// <summary>
	/// This function is called by the photography camera to update the fish in focus map. This is used to update the distance of the fish in focus to the player constantly.
	/// </summary>
	UFUNCTION()
	  void UpdateFishInFocusDistances();

	/// <summary>
	/// Getter to see closest fish in focus currently by checking all distances in the fish in focus map.
	/// </summary>
	/// <returns> The closest fish to player. </returns>
		class ACreature* GetClosestFishInFocus();
		
	// Photo gallery of texturerender2d
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography")
        TArray<class UTextureRenderTarget2D*> PhotoGallery; //UTextureRenderTarget2D

	// Current fish list captured in camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography")
		TMap<class ACreature*, UTextureRenderTarget2D*> CapturedFish; //UTextureRenderTarget2D

	// Current fish in focus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography")
	       TMap<ACreature*, float> FishInFocus;

	// Current camera film
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Photography")
        int CurrentFilm = 20;

	// Max camera film
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography")
		int MaxFilm = 20;

	// Is any fish in focus
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Photography")
		bool bIsAnyFishInFocus = false;

	UPROPERTY()
		FString CameraPhotosFilePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography")
	TArray<TSubclassOf<ACreature>> FishCapturedNico;

	//Moved to gameInstance
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography")
	//int TotalPhotosTaken = 0;
};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 18 2023
Name:		Austin Morris
Summary:	Added photography camera component and base functionality. Storing pictures and fish in focus.
-----------------------------
-----------------------------
Date:	 	Oct 24 2023
Name:		Austin Morris
Summary:	Added a way to see   
-----------------------------
*/
