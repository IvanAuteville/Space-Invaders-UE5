// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvaders/UI/SIBaseUserWidget.h"
#include "SIGameWidget.generated.h"

class UTextBlock;

class UUSIGameInstance;
class AASIGameStateBase;
class AASIGameModeBase;

/**
 * Game Level Widget
 */
UCLASS()
class SPACEINVADERS_API USIGameWidget : public USIBaseUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override final;
	void NativeOnInitialized() override final;
	void NativeDestruct() override final;

private:
	void SetupWidget();
	void SetUpReferences();

	UFUNCTION()
	void SetScore();

	UFUNCTION()
	void SetHIScore();

	UFUNCTION()
	void SetLives();

	UFUNCTION()
	void SetPowerUpLevel();

	UFUNCTION()
	void ShowGameOverText();
	
	UFUNCTION()
	void ShowGameWonText();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> ScoreValueTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> HIScoreValueTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> LivesNumberTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> PowerUpLevelNumberTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> GameOverTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> GameWonTextBlock = nullptr;

	// Instances
	UUSIGameInstance* GameInstance = nullptr;
	AASIGameStateBase* GameState = nullptr;
	AASIGameModeBase* GameMode = nullptr;
};
