// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIInvaderActor.generated.h"

UCLASS()
class SPACEINVADERS_API AASIInvaderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIInvaderActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
