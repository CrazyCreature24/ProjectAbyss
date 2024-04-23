// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BigMenuEyeball.generated.h"

UCLASS()
class PROJECTABYSS_API ABigMenuEyeball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABigMenuEyeball();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyeball")
	class UStaticMeshComponent* FishMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyeball")
	class UStaticMeshComponent* PupilMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 20 2023
Name:		Austin Morris
Summary:	Added base big menu eyeball class and movement functionality with mouse deprojection.
-----------------------------
-----------------------------
Date:	 	Nov 17 2023
Name:		Austin Morris
Summary:	Added a vinterp to the eyeball movement.
-----------------------------
*/