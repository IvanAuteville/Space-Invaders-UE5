// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASIMainMenuGameModeBase.generated.h"

class USIMainMenuWidget;

UCLASS()
class SPACEINVADERS_API AASIMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override final;

private:
	UPROPERTY(Category = "MyClasses", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<USIMainMenuWidget> MainMenuWidgetClass = nullptr;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USIMainMenuWidget* MainMenuWidget = nullptr;
};
