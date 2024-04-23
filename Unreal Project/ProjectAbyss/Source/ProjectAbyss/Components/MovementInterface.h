// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "MovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTABYSS_API IMovementInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void HandleInput(const FInputActionValue& Value);
	virtual void HandleVerticalInput(int Value);
	virtual void Move(FVector InputVector);
	virtual void MoveVertical();

};
