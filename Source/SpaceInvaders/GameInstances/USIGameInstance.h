// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "USIGameInstance.generated.h"

/* Store Info for the whole GameInstance */
UCLASS()
class SPACEINVADERS_API UUSIGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	int32 GetPlayer1HiScore() const { return Player1HiScore; }
	void SetPlayer1HiScore(int32 HiScore) { Player1HiScore = HiScore; }

	int32 GetPlayer1CurrentLevel() const { return Player1CurrentLevel; }
	void SetPlayer1CurrentLevel(int32 Level) { Player1CurrentLevel = Level; }

private:
	UPROPERTY(Category = "Properties", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 Player1HiScore = -1;

	UPROPERTY(Category = "Properties", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 Player1CurrentLevel = 1;
	
};
