// Copyright Ivan Ariel Garcia Auteville.

#pragma once

UENUM(BlueprintType)
enum class EFormationThreatLevel : uint8
{
	None = 0	    UMETA(DisplayName = "None"),
	VeryLow = 1		UMETA(DisplayName = "VeryLow"),
	Low = 2			UMETA(DisplayName = "Low"),
	High = 3		UMETA(DisplayName = "High"),
	VeryHigh = 4	UMETA(DisplayName = "VeryHigh"),
};