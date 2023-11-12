// Copyright Ivan Ariel Garcia Auteville.

#pragma once

UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	None = 0	    	UMETA(DisplayName = "None"),
	SlowTime = 1		UMETA(DisplayName = "SlowTime"),
	StopTime = 2		UMETA(DisplayName = "StopTime"),
    ReverseTime = 3 	UMETA(DisplayName = "ReverseTime"),
};