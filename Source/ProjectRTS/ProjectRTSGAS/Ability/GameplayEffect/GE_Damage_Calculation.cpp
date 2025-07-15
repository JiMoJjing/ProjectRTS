// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_Damage_Calculation.h"

#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

float UGE_Damage_Calculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayEffectContextHandle& ContextHandle = Spec.GetEffectContext();
	const FHitResult* HitResult = ContextHandle.GetHitResult();

	ARTSCharacterPlayer* RTSCharacter = Cast<ARTSCharacterPlayer>(ContextHandle.GetSourceObject());
	if (!RTSCharacter)
	{
		return 0.0f;
	}

	URTSWeaponContext* WeaponContext = RTSCharacter->GetCurrentWeaponContext();
	if (!WeaponContext)
	{
		return 0.0f;
	}

	float BaseDamage = WeaponContext->WeaponDamage;

	if (HitResult->BoneName == FName(TEXT("head")))
	{
		BaseDamage *= 2;
	}
	
	return BaseDamage;
}
