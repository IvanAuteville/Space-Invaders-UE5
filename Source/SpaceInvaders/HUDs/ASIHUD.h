// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ASIHUD.generated.h"

class USIGameWidget;

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AASIHUD : public AHUD
{
	GENERATED_BODY()

protected:
	void BeginPlay() override final;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<USIGameWidget> GameWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere)
	USIGameWidget* GameWidget = nullptr;
};
