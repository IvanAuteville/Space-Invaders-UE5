// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIInvaderActor.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class AASIBaseProjectile;
class AASIInvadersFormation;

UCLASS()
class SPACEINVADERS_API AASIInvaderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIInvaderActor();

	void SetFormation(AASIInvadersFormation* Formation);
	void Move(const FVector& DeltaLocation);
	void LateralBoundReached(const AActor* BoundActorCollidedWith);
//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;

	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> ProjectileSpawnPoint = nullptr;

	UPROPERTY(Category = "Components", EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AASIBaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(Category = "Instance", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	AASIInvadersFormation* MyFormation = nullptr;
};
