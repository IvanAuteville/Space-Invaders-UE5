// Copyright Ivan Ariel Garcia Auteville.

#pragma once

UENUM(BlueprintType)
enum class EInvaderType : uint8
{
	None = 0	    UMETA(DisplayName = "None"),
	Top = 1			UMETA(DisplayName = "Top"),
	Middle = 2	    UMETA(DisplayName = "Middle"),
    Bottom = 3     	UMETA(DisplayName = "Bottom"),
};