// Copyright Ivan Ariel Garcia Auteville.

#pragma once

UENUM(BlueprintType)
enum class EVerticalMovementType : uint8
{
	None = 0	    UMETA(DisplayName = "None"),
	Up = 1			UMETA(DisplayName = "Up"),
	Down = 2	    UMETA(DisplayName = "Down"),
};