// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SIPlayerSpawner.generated.h"

class USceneComponent;

UCLASS()
class SPACEINVADERS_API ASIPlayerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASIPlayerSpawner();

//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SpawnPosition = nullptr;

};
