// Copyright Ivan Ariel Garcia Auteville.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

namespace Utils
{
	namespace InputActionsUtils
	{
		template<class T, class FuncType>
		inline void BindInputStarted(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, FuncType StartedFunc)
		{
			if (!StartedFunc)
				return;

			Input->BindAction(Action, ETriggerEvent::Started, Obj, StartedFunc);
		}

		template<class T, class FuncType>
		inline void BindInputTriggered(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, FuncType TriggeredFunc)
		{
			if (!TriggeredFunc)
				return;

			Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggeredFunc);
		}

		template<class T, class FuncType>
		inline void BindInputCompleted(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, FuncType CompletedFunc)
		{
			if (!CompletedFunc)
				return;

			Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompletedFunc);
		}

		template<class T, class FuncType>
		inline void BindInputStartedTriggeredCompleted(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, 
			FuncType StartedFunc, FuncType TriggeredFunc, FuncType CompletedFunc)
		{
			BindInputStarted(Input, Action, Obj, StartedFunc);
			BindInputTriggered(Input, Action, Obj, TriggeredFunc);
			BindInputCompleted(Input, Action, Obj, CompletedFunc);
		}
	};
}
