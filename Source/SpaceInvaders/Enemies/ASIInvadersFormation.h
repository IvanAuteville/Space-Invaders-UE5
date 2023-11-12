// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIInvadersFormation.generated.h"

class USceneComponent;
class USIFormationDataAsset;
class AASIInvaderActor;

struct FTimerHandle;

UCLASS()
class SPACEINVADERS_API AASIInvadersFormation : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIInvadersFormation();
	
	void Tick(float DeltaTime) override final;
	
	void SpawnInvaders();

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:
	void RunRevealSequence();

	UFUNCTION()
	void InvaderSpawnSequence();

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> FormationLeftRightCornerLocation = nullptr;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USIFormationDataAsset> FormationData = nullptr;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float SpawnSequenceDelayTime = 5.0f;

	UPROPERTY(Category = "Settings", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float MaxSpawnSequenceTime = 0.10f;
	
	UPROPERTY(Category = "Instanced", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<AASIInvaderActor*> Invaders;

	/* Handle to manage the Sequence timer */
	FTimerHandle InvadersSpawnSequenceTimerHandle;
	int32 InvaderSequenceIndex = -1;
};
