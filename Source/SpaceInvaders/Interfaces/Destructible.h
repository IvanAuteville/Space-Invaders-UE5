// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Destructible.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDestructible : public UInterface
{
	GENERATED_BODY()
};

/**
 * Destructible Interface, avoid having to cast to specific Subclasses
 */
class SPACEINVADERS_API IDestructible
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void HandleDestruction(AActor* DestroyerActor) = 0;
};
