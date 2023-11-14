// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ASIPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

// TODO: typo -> double A
UCLASS()
class SPACEINVADERS_API AASIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AASIPlayerController();

	virtual void SetupInputComponent() override final;

protected:
	virtual void BeginPlay() override final;

private:
	void SetInputModeGameAndUI();
	void SetInputModeGameOnly();

	void AddInputMapping();

	UFUNCTION()
	void SelectInputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void PauseGameInputTriggered(const FInputActionValue& Value);

private:
	// Input
	UPROPERTY(Category = "PlayerControllerInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext = nullptr;
	
	UPROPERTY(Category = "PlayerControllerInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SelectInputAction = nullptr;

	UPROPERTY(Category = "PlayerControllerInputs", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> PauseGameInputAction = nullptr;

	UPROPERTY(Category = "GameSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	const TSoftObjectPtr<UWorld> GamePlayLevel = nullptr;

	bool bGamePaused = false; // TODO: check
};
