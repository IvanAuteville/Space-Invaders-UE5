// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvaders/UI/SIBaseUserWidget.h"
#include "SIMainMenuWidget.generated.h"

class UTextBlock;

/**
 * Space Invaders Main Menu Widget
 */
UCLASS()
class SPACEINVADERS_API USIMainMenuWidget : public USIBaseUserWidget
{
	GENERATED_BODY()

protected:
	void NativeOnInitialized() override final;

private:
	void SetupHIScore();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> HIScoreValueTextBlock = nullptr;
};
