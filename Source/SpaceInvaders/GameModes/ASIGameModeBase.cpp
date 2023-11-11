// Copyright Ivan Ariel Garcia Auteville.


#include "ASIGameModeBase.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceInvaders/Actors/SIPlayerSpawner.h"
#include "SpaceInvaders/Actors/SIInvadersSpawner.h"
#include "SpaceInvaders/Player/ASIPlayerPawn.h"
#include "SpaceInvaders/Player/ASIPlayerController.h"

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
	PlayerController = CastChecked<AASIPlayerController>(UGameplayStatics::GetPlayerController(World, 0));

	SpawnPawn(World);

	PlayerController->Possess(Pawn);
}

void AASIGameModeBase::SpawnPawn(UWorld* World)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = PlayerSpawner->GetActorLocation();
	FRotator SpawnRotation = PlayerSpawner->GetActorRotation();

	Pawn = World->SpawnActor<AASIPlayerPawn>(PlayerPawnClass, SpawnLocation, SpawnRotation, SpawnParams);
}
