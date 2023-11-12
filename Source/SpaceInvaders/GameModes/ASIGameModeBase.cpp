// Copyright Ivan Ariel Garcia Auteville.


#include "ASIGameModeBase.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "SpaceInvaders/Actors/SISpawnLocationBase.h"
#include "SpaceInvaders/Player/ASIPlayerPawn.h"
#include "SpaceInvaders/Player/ASIPlayerController.h"
#include "SpaceInvaders/Enemies/ASIUFOActor.h"


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

void AASIGameModeBase::BeginPlay()
{
	UWorld* World = GetWorld();

	PlayerSpawner = GameModeUtils::FindSingleActor(*World, PlayerSpawnerClass);
	InvadersSpawner = GameModeUtils::FindSingleActor(*World, InvadersSpawnerClass);
	LeftUFOSpawner = GameModeUtils::FindSingleActor(*World, LeftUFOSpawnerClass);
	RightUFOSpawner = GameModeUtils::FindSingleActor(*World, RightUFOSpawnerClass);

	PlayerController = CastChecked<AASIPlayerController>(UGameplayStatics::GetPlayerController(World, 0));

	SpawnPawn(World);
	PlayerController->Possess(Pawn);

	// TEST Features:
	// SpawnUFO(World);
}

void AASIGameModeBase::SpawnPawn(UWorld* World)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = PlayerSpawner->GetActorLocation();
	FRotator SpawnRotation = PlayerSpawner->GetActorRotation();

	Pawn = World->SpawnActor<AASIPlayerPawn>(PlayerPawnClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void AASIGameModeBase::SpawnUFO(UWorld* World)
{
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
	const EHorizontalMovementType UFOMovementDir = bSpawnOnLeftSide ? EHorizontalMovementType::Right : EHorizontalMovementType::Left;
	UFO->SetHorizontalMovementDirection(UFOMovementDir);
}
