// Copyright Ivan Ariel Garcia Auteville.

#pragma once

UENUM(BlueprintType)
enum class EHorizontalMovementType : uint8
{
	None = 0	    UMETA(DisplayName = "None"),
	Left = 1		UMETA(DisplayName = "Left"),
	Right = 2	    UMETA(DisplayName = "Right"),
};