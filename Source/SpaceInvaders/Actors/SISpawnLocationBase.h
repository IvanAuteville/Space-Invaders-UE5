// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SISpawnLocationBase.generated.h"

class USceneComponent;

UCLASS()
class SPACEINVADERS_API ASISpawnLocationBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASISpawnLocationBase();

//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SpawnPosition = nullptr;

};