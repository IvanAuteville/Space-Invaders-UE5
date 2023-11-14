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
#include "SpaceInvaders/Enemies/SIFormationDataAsset.h"
#include "SpaceInvaders/Actors/SIInvadersGameOverBound.h"
#include "SpaceInvaders/Projectiles/ASIBaseProjectile.h"
#include "SpaceInvaders/Enemies/ASIInvaderActor.h"

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

	UClass* ActorClass = ASIInvadersGameOverBound::StaticClass();
	ASIInvadersGameOverBound* GameOverBoundActor = GameModeUtils::FindSingleActor<ASIInvadersGameOverBound>(*World, ActorClass);
	GameOverBoundActor->OnInvadersReachedBottom.AddDynamic(this, &ThisClass::OnInvadersReachedPlayerRow);
}

void AASIGameModeBase::GameStart()
{
	MyGameState->SetPlayer1Lives(InitialPlayerLives);
	MyGameState->SetPlayer1Score(MyGameInstance->GetPlayer1CurrentScore());
	MyGameState->SetPlayer1PowerUpLevel(InitialPowerUpLevel);

	SpawnInvadersFormation();

	GetWorld()->GetTimerManager().SetTimer(
		PlayerPawnSpawnTimerHandle,		// handle to cancel timer at a later time
		this,							// the owning object
		&ThisClass::SpawnPawn,			// function to call on elapsed
		PlayerPawnSpawnDelay,			// float delay until elapsed
		false);							// looping?

	SetUFORespawnTimer();
}

void AASIGameModeBase::SpawnPawn()
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = PlayerSpawner->GetActorLocation();
	FRotator SpawnRotation = PlayerSpawner->GetActorRotation();

	MyPawn = World->SpawnActor<AASIPlayerPawn>(PlayerPawnClass, SpawnLocation, SpawnRotation, SpawnParams);
	MyPawn->OnPlayerPawnKilled.AddDynamic(this, &ThisClass::OnPlayerPawnDestroyed);

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

	// Apply Level Offset
	UE_LOG(LogTemp, Warning, TEXT("Player 1 Level: %d"), MyGameInstance->GetPlayer1CurrentLevel());

	int32 CurrentLevel = MyGameInstance->GetPlayer1CurrentLevel();
	CurrentLevel = FMath::Clamp(CurrentLevel, 0, MaxLevel);

	const double OffsetYPerLevel = FormationData->InvadersSpacing.Y;
	const FVector SpawnLocationLevelOffset = FVector(0.0, CurrentLevel * OffsetYPerLevel, 0.0);

	FVector SpawnLocation = InvadersSpawner->GetActorLocation() + SpawnLocationLevelOffset;
	FRotator SpawnRotation = InvadersSpawner->GetActorRotation();

	InvadersFormation = World->SpawnActor<AASIInvadersFormation>(InvadersFormationClass, SpawnLocation, SpawnRotation, SpawnParams);
	InvadersFormation->OnInvadersDefeated.AddDynamic(this, &ThisClass::OnInvadersDefeated);
	InvadersFormation->OnInvaderKilled.AddDynamic(this, &ThisClass::OnInvaderKilled);
}

float AASIGameModeBase::CalculateUFORandomSpawnTime() const
{
	const float RandomDelay = FMath::FRandRange(UFOMinSpawnTime, UFOMaxSpawnTime);
	const float SpawnTime = UFOInitialDelayTime + RandomDelay;
	return SpawnTime;
}

int32 AASIGameModeBase::CalculateUFORandomScore() const
{
	const int32 UFORandomScore = FMath::RandRange(UFOMinScore, UFOMaxScore);
	return UFORandomScore;
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

// TODO: UI ISSUE, maybe delay UFO destruction so we can get the position and Spawn a Label there with the Score
void AASIGameModeBase::OnUFODestroyed(AActor* DestroyerActor)
{
	// Add Score if Killed by the Player
	AASIBaseProjectile* Projectile = Cast<AASIBaseProjectile>(DestroyerActor);
	if (Projectile)
	{
		const int32 UFOScore = CalculateUFORandomScore();
		MyGameState->AddPlayer1Score(UFOScore);
		if (OnScoreUpdated.IsBound())
		{
			OnScoreUpdated.Broadcast();
		}

		UFO->SetRandomScore(UFOScore);

		UE_LOG(LogTemp, Warning, TEXT("UFO SCORE: %d"), UFOScore);
	}

	SetUFORespawnTimer();
}

// NOTE: Called from the Pawn when it is killed from enemy shots only!
void AASIGameModeBase::OnPlayerPawnDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerPawnDestroyed"));

	MyGameState->ReducePlayer1Lives();
	if (OnLivesUpdated.IsBound())
	{
		OnLivesUpdated.Broadcast();
	}

	const int32 PlayerLives = MyGameState->GetPlayer1Lives();
	UE_LOG(LogTemp, Warning, TEXT("Player Lives: %d"), PlayerLives);

	if (PlayerLives <= 0)
	{
		GameOver(EGameOverType::GameLost);
	}
	else
	{
		UWorld* World = GetWorld();

		if (OnGamePaused.IsBound())
		{
			OnGamePaused.Broadcast(true);
		}

		if (!IsValid(UFO))
		{
			World->GetTimerManager().ClearTimer(UFOSpawnTimerHandle);
		}

		World->GetTimerManager().SetTimer(
			PlayerPawnSpawnTimerHandle,			// handle to cancel timer at a later time
			this,								// the owning object
			&ThisClass::ReSpawnPlayerPawn,		// function to call on elapsed
			PlayerPawnSpawnDelay,				// float delay until elapsed
			false);								// looping?
	}
}

void AASIGameModeBase::ReSpawnPlayerPawn()
{
	if (IsValid(UFO))
	{
		UFO->HandleDestruction(this);
	}
	else
	{
		SetUFORespawnTimer();
	}

	SpawnPawn();

	if (OnGamePaused.IsBound())
	{
		OnGamePaused.Broadcast(false);
	}
}

void AASIGameModeBase::OnInvadersReachedPlayerRow()
{
	UE_LOG(LogTemp, Warning, TEXT("OnInvadersReachedPlayerRow"));

	// Rule based on the Original Game
	MyPawn->HandleDestruction(this);

	GameOver(EGameOverType::GameLost);
}

void AASIGameModeBase::OnInvaderKilled(AASIInvaderActor* Invader)
{
	const int32 InvaderScore = ScorePerInvader.FindRef(Invader->GetClass());
	MyGameState->AddPlayer1Score(InvaderScore);

	if (OnScoreUpdated.IsBound())
	{
		OnScoreUpdated.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("INVADER SCORE: %d"), InvaderScore);
}

void AASIGameModeBase::OnInvadersDefeated()
{
	UE_LOG(LogTemp, Warning, TEXT("OnInvadersDefeated"));

	GameOver(EGameOverType::GameWon);
}

void AASIGameModeBase::GameOver(const EGameOverType GameOverType)
{
	CurrentGameOverType = GameOverType;
	
	if (OnGamePaused.IsBound())
	{
		OnGamePaused.Broadcast(true);
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(UFOSpawnTimerHandle);
	const bool bGameWon = (CurrentGameOverType == EGameOverType::GameWon) ? true : false;

	// TODO: Sound
	//LevelAmbientSound->Stop();
	//UGameplayStatics::PlaySound2D(World, bGameWon ? VictorySound : DefeatSound);

	MyGameInstance->SetPlayer1CurrentScore(MyGameState->GetPlayer1Score());
	MyGameInstance->SetPlayer1HiScore(MyGameState->GetPlayer1Score());
	
	// UI Callbacks
	if (OnScoreUpdated.IsBound())
	{
		OnScoreUpdated.Broadcast();
	}
	
	if (OnHIScoreUpdated.IsBound())
	{
		OnHIScoreUpdated.Broadcast();
	}

	if (bGameWon)
	{
		if (OnGameWon.IsBound())
		{
			OnGameWon.Broadcast();
		}
	}
	else
	{
		if (OnGameOver.IsBound())
		{
			OnGameOver.Broadcast();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		LevelTransitionTimerHandle,		// handle to cancel timer at a later time
		this,							// the owning object
		&ThisClass::LevelTransition,	// function to call on elapsed
		LevelTransitionDelayTime,		// float delay until elapsed
		false);							// looping?
}

void AASIGameModeBase::LevelTransition()
{
	UWorld* World = GetWorld();
	const bool bGameWon = (CurrentGameOverType == EGameOverType::GameWon) ? true : false;

	if (bGameWon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Won -> Next Level"));

		if (MyGameInstance->GetPlayer1CurrentLevel() < MaxLevel)
		{
			MyGameInstance->AdvancePlayer1ToNextLevel();
		}

		UGameplayStatics::OpenLevelBySoftObjectPtr(World, GamePlayLevel, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over -> Main Menu"));

		MyGameInstance->ResetPlayer1CurrentLevel();
		MyGameInstance->ResetPlayer1CurrentScore();
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(World, MainMenuLevel, true);
	}
}