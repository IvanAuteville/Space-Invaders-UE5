// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"

namespace Utils
{
	namespace LogUtils
	{
		inline void LogErrorFileAndLine(const ANSICHAR* File, const int32 Line)
		{
			UE_LOG(LogTemp, Error, TEXT("Error in [File:%hs] [Line: %i]"), File, Line);
		}

		inline void LogErrorMsg(const char* Msg)
		{
			FString MsgString (Msg);
			UE_LOG(LogTemp, Error, TEXT("%s"), *MsgString);
		}

		inline void LogWarningMsg(const char* Msg)
		{
			FString MsgString(Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *MsgString);
		}
	};
}
