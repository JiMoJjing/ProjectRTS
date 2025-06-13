// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSInputContext.h"

URTSInputContext::URTSInputContext(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* URTSInputContext::FindNativeActionForTag(const FGameplayTag& InputTag) const
{
	for (const FRTSInputAction& Action : NativeInputActions)
	{
		if (Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	return nullptr;
}

const UInputAction* URTSInputContext::FindAbilityActionForTag(const FGameplayTag& InputTag) const
{
	for (const FRTSInputAction& Action : AbilityInputActions)
	{
		if (Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	return nullptr;
}
