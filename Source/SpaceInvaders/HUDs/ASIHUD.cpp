// Copyright Ivan Ariel Garcia Auteville.


#include "ASIHUD.h"
#include "GameFramework/PlayerController.h"
#include "SpaceInvaders/UI/SIGameWidget.h"

void AASIHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	GameWidget = CreateWidget<USIGameWidget>(PlayerController, GameWidgetClass);
	GameWidget->AddToViewport();
}
