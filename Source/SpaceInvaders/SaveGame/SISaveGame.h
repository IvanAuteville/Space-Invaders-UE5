// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SISaveGame.generated.h"

UCLASS()
class SPACEINVADERS_API USISaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	USISaveGame();

public:
	UPROPERTY(VisibleAnywhere)
	int32 HIScore = -1;
};
