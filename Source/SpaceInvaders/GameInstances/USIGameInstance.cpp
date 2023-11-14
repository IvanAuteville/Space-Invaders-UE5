// Copyright Ivan Ariel Garcia Auteville.


#include "USIGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/SaveGame/SISaveGame.h"

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
    // Retrieve and cast the USaveGame object to USISaveGame.
    if (USISaveGame* LoadedGame = Cast<USISaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex)))
    {
        // Retrieve data from the USISaveGame object.
        Player1HiScore = LoadedGame->HIScore;
        return true;
    }

	return false;
}

bool UUSIGameInstance::SaveGameData()
{
    if (USISaveGame* SaveGameInstance = Cast<USISaveGame>(UGameplayStatics::CreateSaveGameObject(USISaveGame::StaticClass())))
    {
        // Set data on the USISaveGame object.
        SaveGameInstance->HIScore = Player1HiScore;

        // Save the data immediately.
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, SaveUserIndex))
            return true;
    }

	return false;
}
