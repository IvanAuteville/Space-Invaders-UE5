// Copyright Ivan Ariel Garcia Auteville.


#include "SIGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"

#include "SpaceInvaders/GameInstances/USIGameInstance.h"
#include "SpaceInvaders/GameStates/ASIGameStateBase.h"
#include "SpaceInvaders/GameModes/ASIGameModeBase.h"

void USIGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USIGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetupWidget();

	// Bind Delegates
	GameMode->OnScoreUpdated.AddDynamic(this, &ThisClass::SetScore);
	GameMode->OnHIScoreUpdated.AddDynamic(this, &ThisClass::SetHIScore);
	GameMode->OnLivesUpdated.AddDynamic(this, &ThisClass::SetLives);
	GameMode->OnPowerUpLevelUpdated.AddDynamic(this, &ThisClass::SetPowerUpLevel);
	GameMode->OnGameOver.AddDynamic(this, &ThisClass::ShowGameOverText);
	GameMode->OnGameWon.AddDynamic(this, &ThisClass::ShowGameWonText);
}

void USIGameWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// Clear Delegates
	GameMode->OnScoreUpdated.RemoveDynamic(this, &ThisClass::SetScore);
	GameMode->OnHIScoreUpdated.RemoveDynamic(this, &ThisClass::SetHIScore);
	GameMode->OnLivesUpdated.RemoveDynamic(this, &ThisClass::SetLives);
	GameMode->OnPowerUpLevelUpdated.RemoveDynamic(this, &ThisClass::SetPowerUpLevel);
	GameMode->OnGameOver.RemoveDynamic(this, &ThisClass::ShowGameOverText);
	GameMode->OnGameWon.RemoveDynamic(this, &ThisClass::ShowGameWonText);
}

// Fill all values on Init
void USIGameWidget::SetupWidget()
{
	SetUpReferences();

	SetScore();
	SetHIScore();
	SetLives();
	SetPowerUpLevel();
}

void USIGameWidget::SetUpReferences()
{
	GameMode = Cast<AASIGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameState = Cast<AASIGameStateBase>(UGameplayStatics::GetGameState(this));
	GameInstance = Cast<UUSIGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void USIGameWidget::SetScore()
{
	const int32 Score = GameState->GetPlayer1Score();
	ScoreValueTextBlock->SetText(FText::AsNumber(Score));
}

void USIGameWidget::SetHIScore()
{
	const int32 HIScore = GameInstance->GetPlayer1HiScore();
	HIScoreValueTextBlock->SetText(FText::AsNumber(HIScore));
}

void USIGameWidget::SetLives()
{
	const int32 Lives = GameState->GetPlayer1Lives();
	LivesNumberTextBlock->SetText(FText::AsNumber(Lives));
}

void USIGameWidget::SetPowerUpLevel()
{
	const int32 PowerUpLevel = GameState->GetPlayer1PowerUpLevel();
	PowerUpLevelNumberTextBlock->SetText(FText::AsNumber(PowerUpLevel));
}

void USIGameWidget::ShowGameOverText()
{
	GameOverTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void USIGameWidget::ShowGameWonText()
{
	GameWonTextBlock->SetVisibility(ESlateVisibility::Visible);
}
