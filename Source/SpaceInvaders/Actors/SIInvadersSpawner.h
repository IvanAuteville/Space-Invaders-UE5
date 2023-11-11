// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SIInvadersSpawner.generated.h"

UCLASS()
class SPACEINVADERS_API ASIInvadersSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIInvadersSpawner();

//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SpawnPosition = nullptr;

};
