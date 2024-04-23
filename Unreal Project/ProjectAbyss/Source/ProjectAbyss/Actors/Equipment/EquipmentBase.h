// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "EquipmentBase.generated.h"

UCLASS()
class PROJECTABYSS_API AEquipmentBase : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipmentBase();


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Designer|UI")
        UTexture2D* HotbarImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Designer|UI")
		UTexture2D* DefaultImage;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /// <summary>
    /// This is a base method that will be called when the UsePrimaryAction is pressed on the player. This will be overrided for each equipment.
    /// </summary>
    virtual void UsePrimary();

    /// <summary>
    /// This is a base method that will be called when the UsePrimaryAction is released on the player. This will be overrided for each equipment.
    /// </summary>
    virtual void ReleasePrimary();

    /// <summary>
    /// This is a base method that will be called when the UseSecondaryAction is pressed on the player. This will be overrided for each equipment.
    /// </summary>
    virtual void UseSecondary();

    /// <summary>
    /// This is a base method that will be called when the UseSecondaryAction is released on the player. This will be overrided for each equipment.
    /// </summary>
    virtual void ReleaseSecondary();

    /// <summary>
    /// This method is called when you want to detach the equipment form the player. This will go through the players inventory and remove it from it.
    /// </summary>
    virtual void OnDropped();

    /// <summary>
    /// This method is called when the player interacts with this object. It will deactivate the mesh and turn off its collision. It then adds the item to the players hotbar and their inventory, then attaches it to the player via the Equip() method.
    /// </summary>
    virtual void Interacted(ADiverCharacter* Player) override;

    /// <summary>
    /// This method Makes teh interacting actor its owner and then attaches it to the player.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Equipment")
	virtual void Equip(AActor* NewOwner);

    /// <summary>
    /// This method detaches the object and resets the mesh collision to be active.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    virtual void Unequip();

    // Getter and Setter for bIsConsumable
    FORCEINLINE bool GetIsConsumable() const { return bIsConsumable; }
    FORCEINLINE void SetIsConsumable(bool Value) { bIsConsumable = Value; }

    // Getter and Setter for bIsHeld
    FORCEINLINE bool GetIsHeld() const { return bIsHeld; }
    FORCEINLINE void SetIsHeld(bool Value) { bIsHeld = Value; }

    // Getter and Setter for bIsInInventory
    FORCEINLINE bool GetIsInInventory() const { return bIsInInventory; }
    FORCEINLINE void SetIsInInventory(bool Value) { bIsInInventory = Value; }

    // Getter and Setter for bIsPermanent
    FORCEINLINE bool GetIsPermanent() const { return bIsPermanent; }
    FORCEINLINE void SetIsPermanent(bool Value) { bIsPermanent = Value;  }

    // Getter and Setter for Cooldown
    FORCEINLINE float GetCooldown() const { return Cooldown; }
    FORCEINLINE void SetCooldown(float Value) { Cooldown = Value; }

    // Getter and Setter for ExpiryTime
    FORCEINLINE float GetExpiryTime() const { return ExpiryTime; }
    FORCEINLINE void SetExpiryTime(float Value) { ExpiryTime = Value; }

    // Getter and Setter for ShopCost
    FORCEINLINE int32 GetShopCost() const { return ShopCost; }
    FORCEINLINE void SetShopCost(int32 Value) { ShopCost = Value; }

    // Getter and Setter for SellBackPrice
    FORCEINLINE int32 GetSellBackPrice() const { return SellBackPrice; }
    FORCEINLINE void SetSellBackPrice(int32 Value) { SellBackPrice = Value; }

    // Getter and Setter for ItemName
    FORCEINLINE FString GetItemName() const { return ItemName; }
    FORCEINLINE void SetItemName(const FString& Value) { ItemName = Value;  }

    FORCEINLINE FText GetItemDetails() const { return ItemDetails; }
    FORCEINLINE void GetItemDetails(const FText& Value) { ItemDetails = Value; }

    void AttachToBoat();
    void DetachFromBoat();

    // Owning actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        AActor* CurrentOwner;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        bool isAttachedToBoat = true;
protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        bool bIsConsumable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment State")
        bool bIsHeld;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment State")
        bool bIsInInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        bool bIsPermanent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        float Cooldown;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        float ExpiryTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Shop")
        int32 ShopCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Shop")
        int32 SellBackPrice;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
        FText ItemDetails;

public:

    /// <summary>
    /// This method sets the variables so the object can be thrown. Only call this is you have called Unequip() first.
    /// </summary>
    void StartThrow();

    /// <summary>
    /// This method deals with the logic of the items throw along the objects trajectory path.
    /// </summary>
    void ContinueThrow(float DeltaTime);

    //Throwable variables
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
    bool bCanThrow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
    bool bIsThrown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing", meta = (EditCondition = "bCanThrow", EditConditionHides))
    bool bIsCharging;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing", meta = (EditCondition = "bCanThrow", EditConditionHides))
    float ChargeValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing", meta = (EditCondition = "bCanThrow", EditConditionHides))
    float ChargeSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing", meta = (EditCondition = "bCanThrow", EditConditionHides))
    float MinChargeValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing", meta = (EditCondition = "bCanThrow", EditConditionHides))
    float MaxChargeValue;

    // Tragectory path variables
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Designer|Throwing|Tragectory", meta = (EditCondition = "bCanThrow", EditConditionHides))
    TArray<class ATrajectoryObject*> TrajectoryPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing|Tragectory", meta = (EditCondition = "bCanThrow", EditConditionHides))
    TSubclassOf<class ATrajectoryObject> TragectoryTemplate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing|Tragectory", meta = (EditCondition = "bCanThrow", EditConditionHides))
    int WaypointIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Designer|Throwing|Tragectory", meta = (EditCondition = "bCanThrow", EditConditionHides))
    int NumTragectoryMeshes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing|Tragectory", meta = (EditCondition = "bCanThrow", EditConditionHides))
    float TrajectorySimTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Throwing|Tragectory", meta = (EditCondition = "bCanThrow", EditConditionHides))
    float TrajectorySimFrequency;


    /// <summary>
    // If you want to be able to throw the Equipment, place this in BeginPlay(). It creates the pool of objects used for the Trajectory.
    // </summary>
    void CreateObjectPool();

    /// <summary>
    // This method Resets the position of the Trajectory path objects.
    // </summary>
    void ResetTragectory();

    /// <summary>
    // This method disables the mesh on the pooled objects of the Trajectory
    // </summary>
    void TurnOffTrajectory();


};

/*
ChangeLog:
-----------------------------
Date		Nov 14 2023
Name:		Austin Morris
Summary:	Created base class for equipment, added basic variables and functions, added hitbox for equipment, & getters/setters.
-----------------------------
Date		Nov 23 2023
Name:		Alexander McTernan
Summary:	Moved all the Throwing mechanics that I built in the Deployable Light into here. This also includes the Trajectory ability. All the variables, everything so it can be used for any equipment we want.
-----------------------------
Date		Dec 3 2023
Name:		Alexander McTernan
Summary:	Commented all the methods in prep for R2.
-----------------------------
Date		Dec 4 2023
Name:		Alexander McTernan
Summary:	Had to change throw mechanic to deal with the wall clipping bug. Now the object follows the path exactly and it no longer clips.
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added a CallObjectiveID in the equip Method using "PlayerEquippedID". For a tutorial mission
-----------------------------
*/
