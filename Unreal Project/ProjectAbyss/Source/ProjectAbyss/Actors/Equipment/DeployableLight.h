// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipmentBase.h"
#include "DeployableLight.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API ADeployableLight : public AEquipmentBase
{
	GENERATED_BODY()
	
public:

	ADeployableLight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
	class ULightComponent* LightComponent;

	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// This method that will be called when the UsePrimaryAction is pressed on the player. This is overriden from EquipmentBase. It unequips teh object, checks if is being thrown,
	/// And then it removes the equipment from the inventory with the OnDropped() method.
	/// </summary>
	virtual void UsePrimary();

	/// <summary>
	/// This method that will be called when the UsePrimaryAction is released on the player. This is overriden from EquipmentBase. Nothing currently happens here.
	/// </summary>
	virtual void ReleasePrimary();

	/// <summary>
	/// This method that will be called when the UseSecondaryAction is pressed on the player. This is overriden from EquipmentBase. This will reset the trajectory every frame that this is held.
	/// </summary>
	virtual void UseSecondary();

	/// <summary>
	/// This method that will be called when the UseSecondaryAction is released on the player. This is overriden from EquipmentBase. This turns off the visiblity of the trajectory path.
	/// </summary>
	virtual void ReleaseSecondary();

	/// <summary>
	/// This method that will be called when the UsePrimaryAction is released on the player. This is overriden from EquipmentBase. This currently only calls the Super, which removes the item from the inventory.
	/// </summary>
	virtual void OnDropped();

	/// <summary>
	/// This method is called when the player interacts with this object. It will deactivate the mesh and turn off its collision. It then adds the item to the players hotbar and their inventory, then attaches it to the player via the Equip() method.
	/// </summary>
	virtual void Interacted(ADiverCharacter* Player) override;

	//Setters
	void SetLightVisibility(bool bIsVisible);

	//Equip override
	virtual void Equip(AActor* NewOwner) override;

	/// <summary>
	/// This method is subscribed to the MeshComponent. If this equipment detects a rigidbody collision, it will make sure it is not the player or the Water, and then it will disable the throw
	/// </summary>
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};


/*
ChangeLog:
-----------------------------
Date		Nov 14 2023
Name:		Alexander McTernan
Summary:	Initial creation and initialize variables.
			Added LightComponent and the resulting ActivateLight() and SetLightvisiblity().
			Added Charging variables.
			Started to handle collision problems on pickup, but I decided to call it a night before fully fleshed out.
-----------------------------
Date		Nov 20 2023
Name:		Alexander McTernan
Summary:	Fixed collision and attachment issues.
			This object can also now be thrown via holding the right click and pressing left click.
			Started building code to make a visible trajectory.
			Update!
			Added a Tragectory that draws via debug right now.
			Object also stops moving when it hits an object.
			Adding MinChargeValue to assist with readability.
			Future: Need to comment my methods and make more variables Blueprint accessible.
-----------------------------
Date		Nov 22 2023
Name:		Alexander McTernan
Summary:	Finished the trajectory logic by making it use an object pool. Added the functions CreateObjectPool(), ResetTrajectory(), TurnoffTrajectory().
			Also made Dropping and throwing the object into methods. to clean up UsePrimary().
			Moved all the the dropping, throwing, and tragectory logic into the Equipment base so every equipment can use it.
-----------------------------
Date		Dec 6th 2023
Name:		Alexander McTernan
Summary:	Added comments to the methods and removed the ActivateLight() method because it does nothing right now and has no current purpose.
-----------------------------
*/