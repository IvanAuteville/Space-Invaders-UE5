// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASIInvadersBounds.generated.h"

class UBoxComponent;

UCLASS()
class SPACEINVADERS_API AASIInvadersBounds : public AActor
{
	GENERATED_BODY()
	
public:	
	AASIInvadersBounds();

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Component", VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> BoxComp = nullptr;
};
