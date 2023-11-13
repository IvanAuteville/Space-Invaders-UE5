// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASIGameModeBase.generated.h"

class AASIPlayerController;
class AASIPlayerPawn;
class AASIGameStateBase;
class UUSIGameInstance;
class AASIHUD;

class ASISpawnLocationBase;

class AASIUFOActor;
class AASIInvaderActor;
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
	void SetReferences();
	void GameStart();
	void GameOver();

	void SpawnPawn();
	void SpawnUFO();
	void SpawnInvadersFormation();

	float CalculateUFORandomSpawnTime() const;
	void SetUFORespawnTimer();

	UFUNCTION()
	void OnUFODestroyed(AActor* DestroyerActor);

	// TODO: MORE CALLBACKS
	//UFUNCTION()
	//void OnFactionDied(ACSFaction* Faction);

private:
	// Spawners
	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> PlayerSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> InvadersSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> LeftUFOSpawnerClass = nullptr;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASISpawnLocationBase> RightUFOSpawnerClass = nullptr;

	// MyClasses
	UPROPERTY(Category = "MyClasses", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIPlayerPawn> PlayerPawnClass = nullptr;

	UPROPERTY(Category = "MyClasses", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIInvadersFormation> InvadersFormationClass = nullptr;

	UPROPERTY(Category = "MyClasses", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIUFOActor> UFOClass = nullptr;

	// Instances
	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* PlayerSpawner = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* InvadersSpawner = nullptr;
	
	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* LeftUFOSpawner = nullptr;
	
	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ASISpawnLocationBase* RightUFOSpawner = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIUFOActor* UFO = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIInvadersFormation* InvadersFormation = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIPlayerController* MyPlayerController = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIPlayerPawn* MyPawn = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIGameStateBase* MyGameState = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	UUSIGameInstance* MyGameInstance = nullptr;

	UPROPERTY(Category = "Instances", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIHUD* MyHUD = nullptr;

	// Timers

	/* Pawn Spawn Timer */
	FTimerHandle PlayerPawnSpawnTimerHandle;

	UPROPERTY(Category = "Spawners", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float PlayerPawnSpawnDelay = 4.0f;

	/* UFO Spawn Timer */
	FTimerHandle UFOSpawnTimerHandle;

	// UFOSettings
	UPROPERTY(Category = "UFOSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float UFOInitialDelayTime = 30.0f;

	UPROPERTY(Category = "UFOSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float UFOMinSpawnTime = 10.0f;

	UPROPERTY(Category = "UFOSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float UFOMaxSpawnTime = 20.0f;

	// ScoreSettings
	UPROPERTY(Category = "ScoreSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<AASIInvaderActor>, int32> ScorePerInvader;

	UPROPERTY(Category = "ScoreSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 UFOScore = 100;

	// GameSettings
	UPROPERTY(Category = "GameSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 InitialPlayerLives = 3;

	UPROPERTY(Category = "GameSettings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 InitialPowerUpLevel = 0;
};
