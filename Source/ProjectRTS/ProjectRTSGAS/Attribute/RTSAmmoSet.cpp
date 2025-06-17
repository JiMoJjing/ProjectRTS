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

	FString PrintString = FString::Printf(TEXT("%d / %d"), (uint8)GetCurrentAmmo(), (uint8)GetMaxAmmo());
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, *PrintString);
}
