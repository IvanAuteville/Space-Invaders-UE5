// Copyright Ivan Ariel Garcia Auteville.


#include "ASIMainMenuGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "SpaceInvaders/Player/ASIPlayerController.h"
#include "SpaceInvaders/UI/SIMainMenuWidget.h"

// TODO: this should be done by the HUD class, but I don't have time now!
void AASIMainMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	AASIPlayerController* PlayerController = Cast<AASIPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	MainMenuWidget = CreateWidget<USIMainMenuWidget>(PlayerController, MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}
