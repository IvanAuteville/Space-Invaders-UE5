// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SIFormationDataAsset.generated.h"

class AASIInvaderActor;

// Custom structure to represent a pair of values, TPair like
USTRUCT(BlueprintType)
struct FInvaderTypeRowPair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AASIInvaderActor> InvaderType = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 AmountOfRows = 0;
};

UCLASS(Const)
class SPACEINVADERS_API USIFormationDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FPrimaryAssetId GetPrimaryAssetId() const override final
	{
		return FPrimaryAssetId(GetClass()->GetFName(), GetFName());
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInvaderTypeRowPair> InvaderTypeRowsToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AmountOfInvadersPerRow = 11;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D InvadersSpacing = FVector2D(0.0);
};
