// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASIPlayerPawn.generated.h"

UCLASS()
class SPACEINVADERS_API AASIPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AASIPlayerPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
