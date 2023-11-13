// Copyright Ivan Ariel Garcia Auteville.


#include "USIGameInstance.h"

void UUSIGameInstance::Init()
{
	Super::Init();
	LoadGameData();
}

void UUSIGameInstance::Shutdown()
{
	Super::Shutdown();
	SaveGameData();
}

bool UUSIGameInstance::LoadGameData()
{

	return true;
}

bool UUSIGameInstance::SaveGameData()
{

	return true;
}
