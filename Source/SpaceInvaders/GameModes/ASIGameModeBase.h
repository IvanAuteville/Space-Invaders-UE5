// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASIGameModeBase.generated.h"

class ASISpawnLocationBase;
class AASIPlayerController;
class AASIPlayerPawn;
class AASIUFOActor;
class AASIInvadersFormation;

struct FTimerHandle;

UCLASS()
class SPACEINVADERS_API AASIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:
	void SpawnPawn(UWorld& World);
	void SpawnUFO();
	void SpawnInvadersFormation(UWorld& World);

private:
	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> PlayerSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> InvadersSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> LeftUFOSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> RightUFOSpawnerClass = nullptr;

	UPROPERTY(Category = "PlayerPawn", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIPlayerPawn> PlayerPawnClass = nullptr;

	UPROPERTY(Category = "Enemies", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIInvadersFormation> InvadersFormationClass = nullptr;

	UPROPERTY(Category = "Enemies", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIUFOActor> UFOClass = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* PlayerSpawner = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* InvadersSpawner = nullptr;
	
	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* LeftUFOSpawner = nullptr;
	
	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* RightUFOSpawner = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIPlayerController* PlayerController = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIPlayerPawn* Pawn = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIUFOActor* UFO = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIInvadersFormation* InvadersFormation = nullptr;

	// -- TEST --
	/* Handle to manage the UFO Spawn timer */
	FTimerHandle UFOTestTimerHandle;
	
	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float TestSpawnUFODelayTime = 10.0f;
	// -- END OF TEST --
};
