// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Creature.generated.h"

//Creature type enum
UENUM(BlueprintType)
enum class ECreatureType : uint8
{
	CT_Fish UMETA(DisplayName = "Fish"),
	CT_Plant UMETA(DisplayName = "Plant"),
	CT_Remnants UMETA(DisplayName = "Remnants"),
	CT_BowlingBalls UMETA(DisplayName = "BowlingBalls"),
	CT_MAX UMETA(DisplayName = "DefaultMAX")
};


UCLASS(Blueprintable)
class PROJECTABYSS_API ACreature : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACreature();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
		class UStaticMeshComponent* Mesh;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
		class UWidgetComponent* FocusWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
		class UWidgetComponent* ScannedWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestIDComponent") //Nico Modification
		class UMissionObjectiveIDComponent* ObjectiveIDsComponent;

	// Niagara component for the fish bubbles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
		class UNiagaraComponent* BubblesNiagaraComponent;

	// Creature type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Type")
		ECreatureType CreatureType = ECreatureType::CT_Fish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Textures")
		UTexture2D* RedCameraBoxTexture;

	// Blue camera box texture subclass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Textures")
		UTexture2D* BlueCameraBoxTexture;

	// Image of the fish for card icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Textures")
		UTexture2D* IconImage;

	// Image of the fish for profile card
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Textures")
		UTexture2D* ProfileImage;

	// Default Image of the fish for profile icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Textures")
		UTexture2D* IconImageDefault;

	// Default Image of the fish for profile card
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Textures")
		UTexture2D* ProfileImageDefault;

	// Bool for isinfocus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creature")
		bool bIsInFocus = false;

	// Focus widget for the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Classes")
		TSubclassOf<class UInFocusWidget> FocusWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Classes")
		TSubclassOf<class UScannedWidget> ScannedWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Particles", BlueprintSetter=SetHasBubbles)
		bool bHasBubbles = false;

	//Name of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString Name = "???";

	//Scientific name of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString ScientificName = "???";

	//Description of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString PhysicalDescription;

	//Weight class of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString Weight;

	//Length of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString Length;

	//Habitat of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString Habitat;

	//Diet of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString Diet;

	//Behaviors of the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		FString Behaviors;


	//Fish collection index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		int CollectionIndex = 0;

	// Is the fish bonus available
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		bool bFirstTimeBonusAvailable = true;

	// First time bonus rewarded in currency
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics|Value")
		int FirstTimeBonus = 300;

	// Default fish vlaue rewarded in currency
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics|Value")
		int MoneyValue = 100;

	// Fish picture cooldown till you can take a picture of the fish again

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		float PictureCooldown = 60.0f;

	// Fish picture cooldown till you can take a picture of the fish again
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Statistics")
		bool bIsPictureCooldown = false;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Damage")
		float KnockBack = 850.f;

	// Fish timer handle 
	FTimerHandle PictureCooldownTimerHandle;
	class ADiverCharacter* DiverRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Override post edit change
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Setter for has bubbles
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void SetHasBubbles(bool HasBubbles);

	// Getter for isinfocus
	UFUNCTION(BlueprintCallable, Category = "Creature")
		bool GetIsInFocus();

	// Setter for isinfocus
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void SetIsInFocus(bool bInFocus);

	UFUNCTION(BlueprintCallable, Category = "Creature")
		void SetCaptured();

	// Setter for picture cooldown
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void SetPictureCooldown();

	// Getter for picture cooldown
	UFUNCTION(BlueprintCallable, Category = "Creature")
		bool GetPictureCooldown();

	// Show focus widget
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void ShowFocusWidget();

	// Hide focus widget
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void HideFocusWidget();

	// Spawn scanned widget
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void ShowScannedWidget();

	// Hide scanned widget
	UFUNCTION(BlueprintCallable, Category = "Creature")
		void HideScannedWidget();

	// Play scanned animation
	UFUNCTION(BlueprintCallable, Category = "Creature")
        void PlayScannedAnimation();

	UFUNCTION(BlueprintCallable, Category = "Creature")
		void StartCooldownTimer();


};
