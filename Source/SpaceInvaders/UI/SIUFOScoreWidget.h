// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvaders/UI/SIBaseUserWidget.h"
#include "SIUFOScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class SPACEINVADERS_API USIUFOScoreWidget : public USIBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetScore(const int32 Score);

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> ScoreTextBlock = nullptr;
};
