// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSWeaponBar.h"

#include "Components/TextBlock.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAmmoSet.h"

void URTSWeaponBar::NativeConstruct()
{
	Super::NativeConstruct();

	URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponent();
	if (!RTSASC)
	{
		return;
	}

	const URTSAmmoSet* RTSAmmoSet = RTSASC->GetSet<URTSAmmoSet>();
	if (!RTSAmmoSet)
	{
		return;
	}

	RTSAmmoSet->OnAmmoChanged.AddUObject(this, &URTSWeaponBar::OnAmmoChanged);
	OnAmmoChanged(RTSAmmoSet->GetCurrentAmmo(), RTSAmmoSet->GetMaxAmmo());
}

void URTSWeaponBar::OnAmmoChanged(float InCurrentAmmo, float InMaxAmmo)
{
	FString Ammo = FString::Printf(TEXT("%d / %d"), (uint8)InCurrentAmmo, (uint8)InMaxAmmo);
	AmmoText->SetText(FText::FromString(Ammo));
}
