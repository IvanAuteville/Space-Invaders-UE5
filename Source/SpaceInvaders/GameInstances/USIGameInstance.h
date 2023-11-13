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
	void Init() override final;
	void Shutdown() override final;

	int32 GetPlayer1HiScore() const { return Player1HiScore; }
	void SetPlayer1HiScore(const int32 HiScore)
	{
		if (HiScore < Player1HiScore)
			return;

		Player1HiScore = HiScore;
	}

	int32 GetPlayer1CurrentScore() const { return Player1CurrentScore; }
	void SetPlayer1CurrentScore(const int32 Score) { Player1CurrentScore = Score; }
	void ResetPlayer1CurrentScore() { Player1CurrentScore = 0; }

	int32 GetPlayer1CurrentLevel() const { return Player1CurrentLevel; }
	void SetPlayer1CurrentLevel(const int32 Level) { Player1CurrentLevel = Level; }
	void AdvancePlayer1ToNextLevel() { ++Player1CurrentLevel; }
	void ResetPlayer1CurrentLevel() { Player1CurrentLevel = 0; }

private:
	UFUNCTION()
	bool LoadGameData();

	UFUNCTION()
	bool SaveGameData();

private:
	UPROPERTY(Category = "Properties", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 Player1CurrentScore = 0;

	UPROPERTY(Category = "Properties", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 Player1HiScore = 0;

	UPROPERTY(Category = "Properties", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 Player1CurrentLevel = 0;
	
};
