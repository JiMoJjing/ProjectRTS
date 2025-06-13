// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "RTSInputContext.h"
#include "RTSInputComponent.generated.h"

class URTSInputContext;

/**
 * Author		: 지용현
 * Date			: 2025.05.28
 * Description	: 데이터 애셋을 받아 입력 바인드처리 기능을 추가한 InputComponent.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTRTS_API URTSInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	URTSInputComponent(const FObjectInitializer& ObjectInitializer);

	template<class UserClass, typename FuncType>
	void BindNativeAction(const URTSInputContext* InputDataAsset, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityAction(const URTSInputContext* InputDataAsset, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);

	template<class UserClass, typename FuncType>
	void BindWeaponSwapAction(const URTSInputContext* InputDataAsset, UserClass* Object, FuncType Func);
};

template <class UserClass, typename FuncType>
void URTSInputComponent::BindNativeAction(const URTSInputContext* InputDataAsset, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputDataAsset);

	if (const UInputAction* IA = InputDataAsset->FindNativeActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void URTSInputComponent::BindAbilityAction(const URTSInputContext* InputDataAsset, UserClass* Object,	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputDataAsset);

	for (const FRTSInputAction& Action : InputDataAsset->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}

template <class UserClass, typename FuncType>
void URTSInputComponent::BindWeaponSwapAction(const URTSInputContext* InputDataAsset, UserClass* Object, FuncType Func)
{
	uint8 ArraySize = InputDataAsset->WeaponSwapInputActions.Num();
	
	for (uint8 ix = 0; ix < ArraySize; ++ix)
	{
		BindAction(InputDataAsset->WeaponSwapInputActions[ix], ETriggerEvent::Started, Object, Func, ix);
	}
}
