// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAmmoSet.h"

URTSAmmoSet::URTSAmmoSet(const FObjectInitializer& ObjectInitializer)
{
	CurrentAmmo = 0;
	MaxAmmo = 0;
}

void URTSAmmoSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// FString PrintString = FString::Printf(TEXT("%d / %d"), (uint8)GetCurrentAmmo(), (uint8)GetMaxAmmo());
	// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, *PrintString);
}

void URTSAmmoSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void URTSAmmoSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void URTSAmmoSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	OnAmmoChanged.Broadcast(GetCurrentAmmo(), GetMaxAmmo());
}

void URTSAmmoSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetCurrentAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAmmo());
	}
	else if (Attribute == GetMaxAmmoAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

