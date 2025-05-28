// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSInputDataAsset.h"

URTSInputDataAsset::URTSInputDataAsset(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* URTSInputDataAsset::FindNativeActionForTag(const FGameplayTag& InputTag) const
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

const UInputAction* URTSInputDataAsset::FindAbilityActionForTag(const FGameplayTag& InputTag) const
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
