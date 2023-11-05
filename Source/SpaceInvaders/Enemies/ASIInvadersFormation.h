// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIInvadersFormation.generated.h"

UCLASS()
class SPACEINVADERS_API AASIInvadersFormation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASIInvadersFormation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
