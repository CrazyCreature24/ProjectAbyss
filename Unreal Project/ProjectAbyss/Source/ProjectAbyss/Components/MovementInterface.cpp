// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/MovementInterface.h"
#include "MovementInterface.h"

// Add default functionality here for any IMovementInterface functions that are not pure virtual.

void IMovementInterface::HandleInput(const FInputActionValue& Value)
{
}

void IMovementInterface::HandleVerticalInput(int Value)
{
}

void IMovementInterface::Move(FVector InputVector)
{
}

void IMovementInterface::MoveVertical()
{

}
