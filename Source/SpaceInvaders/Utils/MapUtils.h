// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"

/**
 * TMap Utilities
 */
namespace Utils
{
	namespace MapUtils
	{
		template<typename KeyType, typename ValueType>
		inline TArray<const KeyType*> GetConstMapKeys(const TMap<KeyType*, ValueType*>& Map)
		{
			TArray<const KeyType*> Keys;
			for (const auto& Entry : Map)
			{
				Keys.Add(Entry.Key);
			}
			return Keys;
		}

		template<typename KeyType, typename ValueType>
		inline TArray<const ValueType*> GetConstMapValues(const TMap<KeyType*, ValueType*>& Map)
		{
			TArray<const ValueType*> Values;
			for (const auto& Entry : Map)
			{
				Values.Add(Entry.Value);
			}
			return Values;
		}
	};
}

