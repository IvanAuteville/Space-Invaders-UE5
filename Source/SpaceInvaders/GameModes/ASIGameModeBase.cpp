// Copyright Ivan Ariel Garcia Auteville.


#include "ASIGameModeBase.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "SpaceInvaders/Actors/SISpawnLocationBase.h"
#include "SpaceInvaders/Enemies/ASIUFOActor.h"
#include "SpaceInvaders/Enemies/ASIInvadersFormation.h"

#include "SpaceInvaders/Player/ASIPlayerPawn.h"
#include "SpaceInvaders/Player/ASIPlayerController.h"
#include "SpaceInvaders/GameStates/ASIGameStateBase.h"
#include "SpaceInvaders/GameInstances/USIGameInstance.h"
#include "SpaceInvaders/HUDs/ASIHUD.h"

// TODO: Move to Utils
namespace GameModeUtils
{
	template<typename Type>
	inline Type* FindSingleActor(const UWorld& World, const TSubclassOf<Type> ActorClass)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(&World, ActorClass, FoundActors);

		// We expect only one of this type
		if (FoundActors.Num() != 1)
			return nullptr;

		return CastChecked<Type>(FoundActors[0]);
	}
}
//---

void AASIGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	GameStart();
}

void AASIGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AASIGameModeBase::SetReferences()
{
	UWorld* World = GetWorld();

	PlayerSpawner = GameModeUtils::FindSingleActor(*World, PlayerSpawnerClass);
	InvadersSpawner = GameModeUtils::FindSingleActor(*World, InvadersSpawnerClass);
	LeftUFOSpawner = GameModeUtils::FindSingleActor(*World, LeftUFOSpawnerClass);
	RightUFOSpawner = GameModeUtils::FindSingleActor(*World, RightUFOSpawnerClass);
	MyPlayerController = CastChecked<AASIPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
	MyGameState = Cast<AASIGameStateBase>(UGameplayStatics::GetGameState(World));
	MyGameInstance = Cast<UUSIGameInstance>(UGameplayStatics::GetGameInstance(World));
	MyHUD = Cast<AASIHUD>(MyPlayerController->GetHUD());
}

void AASIGameModeBase::GameStart()
{
	SpawnInvadersFormation();

	GetWorld()->GetTimerManager().SetTimer(
		PlayerPawnSpawnTimerHandle,		// handle to cancel timer at a later time
		this,							// the owning object
		&ThisClass::SpawnPawn,			// function to call on elapsed
		PlayerPawnSpawnDelay,			// float delay until elapsed
		false);							// looping?

	SetUFORespawnTimer();
}

void AASIGameModeBase::GameOver()
{
	// TODO: Sound
	//LevelAmbientSound->Stop();
	//UGameplayStatics::PlaySound2D(GetWorld(), PlayerFaction == LoserFaction ? DefeatSound : VictorySound);

	//if (PlayerFaction != LoserFaction)
	//{
	//	HUD->ShowCombatVictoryMenu();
	//}
	//else
	//{
	//	HUD->ShowCombatDefeatMenu();
	//}
}

void AASIGameModeBase::SpawnPawn()
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = PlayerSpawner->GetActorLocation();
	FRotator SpawnRotation = PlayerSpawner->GetActorRotation();

	MyPawn = World->SpawnActor<AASIPlayerPawn>(PlayerPawnClass, SpawnLocation, SpawnRotation, SpawnParams);

	MyPlayerController->Possess(MyPawn);
}

void AASIGameModeBase::SpawnUFO()
{
	UWorld* World = GetWorld();

	World->GetTimerManager().ClearTimer(UFOSpawnTimerHandle);

	// We already have an UFO in the Scene 
	if (IsValid(UFO))
		return;

	const bool bSpawnOnLeftSide = FMath::RandBool();
	ASISpawnLocationBase* UFOSpawnLocation = bSpawnOnLeftSide ? LeftUFOSpawner : RightUFOSpawner;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = UFOSpawnLocation->GetActorLocation();
	FRotator SpawnRotation = UFOSpawnLocation->GetActorRotation();

	UFO = World->SpawnActor<AASIUFOActor>(UFOClass, SpawnLocation, SpawnRotation, SpawnParams);
	UFO->OnUFODestroyed.AddDynamic(this, &ThisClass::OnUFODestroyed);
	
	const EHorizontalMovementType UFOMovementDir = bSpawnOnLeftSide ? EHorizontalMovementType::Right : EHorizontalMovementType::Left;
	UFO->SetHorizontalMovementDirection(UFOMovementDir);
}

void AASIGameModeBase::SpawnInvadersFormation()
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = InvadersSpawner->GetActorLocation();
	FRotator SpawnRotation = InvadersSpawner->GetActorRotation();

	InvadersFormation = World->SpawnActor<AASIInvadersFormation>(InvadersFormationClass, SpawnLocation, SpawnRotation, SpawnParams);
}

float AASIGameModeBase::CalculateUFORandomSpawnTime() const
{
	const float RandomDelay = FMath::FRandRange(UFOMinSpawnTime, UFOMaxSpawnTime);
	const float SpawnTime = UFOInitialDelayTime + RandomDelay;
	return SpawnTime;
}

void AASIGameModeBase::SetUFORespawnTimer()
{
	const float UFODelayTime = CalculateUFORandomSpawnTime();
	GetWorld()->GetTimerManager().SetTimer(
		UFOSpawnTimerHandle,			// handle to cancel timer at a later time
		this,							// the owning object
		&ThisClass::SpawnUFO,			// function to call on elapsed
		UFODelayTime,					// float delay until elapsed
		false);							// looping?
}

void AASIGameModeBase::OnUFODestroyed(AActor* DestroyerActor)
{
	// Score if Killed by player
	// GameState

	SetUFORespawnTimer();
}
