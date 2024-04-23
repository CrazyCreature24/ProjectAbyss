// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateTransitionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStateTransitionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTABYSS_API IStateTransitionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void TransitionMovementStates();
	virtual void SetToIsSwimming();
};
