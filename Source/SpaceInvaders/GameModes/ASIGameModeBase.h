// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASIGameModeBase.generated.h"

class ASIPlayerSpawner;
class ASIInvadersSpawner;
class AASIPlayerPawn;
class AASIPlayerController;

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AASIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override final;

	void SpawnPawn(UWorld* World);

private:
	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASIPlayerSpawner> PlayerSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASIInvadersSpawner> InvadersSpawnerClass = nullptr;

	UPROPERTY(Category = "PlayerPawn", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIPlayerPawn> PlayerPawnClass = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASIPlayerSpawner* PlayerSpawner = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASIInvadersSpawner* InvadersSpawner = nullptr;
	
	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIPlayerController* PlayerController = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIPlayerPawn* Pawn = nullptr;
};
