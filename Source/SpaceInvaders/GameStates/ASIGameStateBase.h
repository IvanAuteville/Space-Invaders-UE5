// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ASIGameStateBase.generated.h"

UCLASS()
class SPACEINVADERS_API AASIGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	int32 GetPlayer1Lives() const { return Player1Lives; }
	void SetPlayer1Lives(const int32 Lives) { Player1Lives = Lives; }
	void ReducePlayer1Lives() { --Player1Lives; }

	int32 GetPlayer1Score() const { return Player1Score; }
	void SetPlayer1Score(const int32 Score) { Player1Score = Score; }
	void AddPlayer1Score(const int32 Score) { Player1Score += Score; }

	int32 GetPlayer1PowerUpLevel() const { return PowerUpLevel; }
	void SetPlayer1PowerUpLevel(const int32 Level) { PowerUpLevel = Level; }
	void AddPlayer1PowerUpLevel() { ++PowerUpLevel; }
	void ResetPlayer1PowerUpLevel() { PowerUpLevel = 0; }

private:
	UPROPERTY(Category = "Properties", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 Player1Lives = -1;

	UPROPERTY(Category = "Properties", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 Player1Score = -1;

	UPROPERTY(Category = "Properties", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 PowerUpLevel = 0;
};
