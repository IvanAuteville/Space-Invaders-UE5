// Copyright Ivan Ariel Garcia Auteville.


#include "SIMainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"
#include "SpaceInvaders/GameInstances/USIGameInstance.h"

void USIMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetupHIScore();
}

void USIMainMenuWidget::SetupHIScore()
{
	UUSIGameInstance* GameInstance = Cast<UUSIGameInstance>(UGameplayStatics::GetGameInstance(this));
	const int32 HIScore = GameInstance->GetPlayer1HiScore();
	HIScoreValueTextBlock->SetText(FText::AsNumber(HIScore));
}
