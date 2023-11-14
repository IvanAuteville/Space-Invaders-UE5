// Copyright Ivan Ariel Garcia Auteville.


#include "SIUFOScoreWidget.h"
#include "UMG/Public/Components/TextBlock.h"

void USIUFOScoreWidget::SetScore(const int32 Score)
{
	ScoreTextBlock->SetText(FText::AsNumber(Score));
}
