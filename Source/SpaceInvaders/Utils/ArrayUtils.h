// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"

/**
 * TArray Utilities
 */
namespace Utils
{
	namespace ArrayUtils
	{
		template<typename Type>
		inline FMD5Hash GetArrayHash(const TArray<const Type*>& Array)
		{
			FMD5Hash Hash;

			FMD5 Gen;
			Gen.Update(reinterpret_cast<const uint8*>(Array.GetData()),
				Array.Num() * Array.GetTypeSize());
			Hash.Set(Gen);

			return Hash;
		}

		template<typename Type>
		inline FMD5Hash GetArrayHash(const TArray<Type*>& Array)
		{
			FMD5Hash Hash;

			FMD5 Gen;
			Gen.Update(reinterpret_cast<const uint8*>(Array.GetData()),
				Array.Num() * Array.GetTypeSize());
			Hash.Set(Gen);

			return Hash;
		}

		template<typename Type>
		inline TArray<const Type*> GetConstArray(const TArray<Type*>& InArray)
		{
			TArray<const Type*> OutArray;
			for (const auto& Element : InArray)
			{
				OutArray.Add(Element);
			}
			return OutArray;
		}
	};
};

