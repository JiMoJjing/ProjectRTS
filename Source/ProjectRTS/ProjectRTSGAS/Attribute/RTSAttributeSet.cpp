// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"

URTSAttributeSet::URTSAttributeSet()
	: Health(100.0f), MaxHealth(100.0f)
{
	bOutOfHealth = false;
	MaxHealthBeforeAttributeChange = 0.0f;
	HealthBeforeAttributeChange = 0.0f;
}

URTSAbilitySystemComponent* URTSAttributeSet::GetRTSAbilitySystemComponent() const
{
	return Cast<URTSAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

void URTSAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URTSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URTSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void URTSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	// 실제 값 변했는지 체크, 변했으면 AttributeChangeDelegate 호출, Clamping 처리.
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, Health, OldValue);

	// Delegate Broadcast용 변수 설정 및 Broadcast.
	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();

	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void URTSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, MaxHealth, OldValue);
}

bool URTSAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void URTSAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void URTSAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void URTSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void URTSAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void URTSAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
}
