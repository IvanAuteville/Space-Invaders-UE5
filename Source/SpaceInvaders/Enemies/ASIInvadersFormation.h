// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SpaceInvaders/Enums/FormationThreatLevel.h"
#include "SpaceInvaders/Enums/HorizontalMovementType.h"
#include "SpaceInvaders/Enums/VerticalMovementType.h"
#include "ASIInvadersFormation.generated.h"

class USceneComponent;
class USIFormationDataAsset;
class AASIInvaderActor;
class AASIBaseProjectile;

struct FTimerHandle;

USTRUCT(BlueprintType)
struct FInvaderFormationSlot
{
	GENERATED_BODY()

public:
	FInvaderFormationSlot() = default;

	FInvaderFormationSlot(const int32 NewCol, const int32 NewRow, AASIInvaderActor* NewInvader = nullptr)
		: Column(NewCol), Row(NewRow), Invader(NewInvader) {};

public:
	int32 Column = -1;
	int32 Row = -1;
	AASIInvaderActor* Invader = nullptr;
};

// TODO: Move to Enums folder
/* Very simple StateMachine like */
UENUM(BlueprintType)
enum class EInvadersFormationState : uint8
{
	None = 0	    	UMETA(DisplayName = "None"),
	Spawning = 1		UMETA(DisplayName = "Spawning"),
	Moving = 2			UMETA(DisplayName = "Moving"),
	Slowed = 3			UMETA(DisplayName = "Slowed"),
	Stopped = 4			UMETA(DisplayName = "Stopped"),
	Reverse = 5			UMETA(DisplayName = "Reverse"),
};

// TODO: Move to Enums folder
UENUM(BlueprintType)
enum class EInvadersFormationAxisMovement : uint8
{
	None = 0	    	UMETA(DisplayName = "None"),
	Horizontal = 1		UMETA(DisplayName = "Horizontal"),
	Vertical = 2		UMETA(DisplayName = "Vertical"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvadersDefeated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvaderKilled, AASIInvaderActor*, Invader);

UCLASS()
class SPACEINVADERS_API AASIInvadersFormation : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIInvadersFormation();
	void Tick(float DeltaTime) override final;
	
	void SpawnInvaders();
	void LateralBoundReached(const AActor* BoundActorCollidedWith);

	void InvaderDestroyed(AASIInvaderActor* Invader);

	UFUNCTION()
	void OnPlayerPawnDestroyed();

public:
	// Delegates
	UPROPERTY()
	FOnInvadersDefeated OnInvadersDefeated;

	UPROPERTY()
	FOnInvaderKilled OnInvaderKilled;

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:
	void SpawnProjectiles();
	float CalculateRandomFireTime() const;
	void TryToFire();
	void RunFireTimer();

	AASIInvaderActor* FindInvaderToFire();
	FInvaderFormationSlot* FindInvaderFormationSlot(AASIInvaderActor* Invader);

	bool InvaderHasFreePathToFire(const FInvaderFormationSlot& FormationSlot);
	int32 CalcUnitIndex(const int32 Row, const int32 Col) const;

	UFUNCTION()
	void OnProjectileReady(AASIBaseProjectile* Projectile);

	void RunRevealSequence();
	UFUNCTION()
	void InvaderSpawnSequence();

	UFUNCTION()
	void OnGamePaused(const bool bPaused);

	bool ShouldMove() const;
	void Move(const float DeltaTime);

	bool IsDestinationRowReached() const;
	void UpdateCurrentRow();
	void UpdateDestinationRow();

	void UpdateFormationGridOnInvaderDestroyed(AASIInvaderActor* Invader);
	void UpdateFormationThreatLevel();

private:
	UPROPERTY(Category = "Component", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> FormationLeftRightCornerLocation = nullptr;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USIFormationDataAsset> FormationData = nullptr;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIBaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float SpawnSequenceDelayTime = 5.0f;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MaxSpawnSequenceTime = 0.10f;

	// TODO: Calculate or cache Movement Speed base on EFormationThreatLevel
	// Could be a DataAsset or DataTable too
	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	double BaseMovementSpeed = 100.0;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	double SlowMultiplier = 0.5;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 NumberOfProjectiles = 4;
	
	/* All available Invaders in the Formation */
	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<AASIInvaderActor*> Invaders;

	/* Grid like Formation, util for lots of operations */
	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<FInvaderFormationSlot> FormationGrid;
	
	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EInvadersFormationState InvadersFormationState = EInvadersFormationState::None;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EInvadersFormationAxisMovement InvadersFormationAxisMovement = EInvadersFormationAxisMovement::Horizontal;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EHorizontalMovementType HorizontalMovementType = EHorizontalMovementType::Right;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EVerticalMovementType VerticalMovementType = EVerticalMovementType::Down;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EFormationThreatLevel FormationThreatLevel = EFormationThreatLevel::VeryLow;
	
	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	double CurrentRowY = 0.0;
	
	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	double DestinationRowY = 0.0;
	
	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	const AActor* LastBoundActorCollidedWith = nullptr;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<AASIBaseProjectile>> ProjectilesReady;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<AASIBaseProjectile>> ProjectilesInUse;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 OriginalInvadersCount = -1;

	/* Handle to manage the Sequence timer */
	FTimerHandle InvadersSpawnSequenceTimerHandle;
	int32 InvaderSequenceIndex = -1;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MinShootTime = 1.0f;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MaxShootTime = 5.0f;

	/* Handle to manage Firing */
	FTimerHandle FireTimerHandle;
};
