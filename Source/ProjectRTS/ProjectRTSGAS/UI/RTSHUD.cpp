// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSHUD.h"

#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/RTSGameplayTag.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

URTSHUD::URTSHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}


void URTSHUD::NativeConstruct()
{
	Super::NativeConstruct();

	DefaultCrosshair->SetVisibility(ESlateVisibility::Visible);
	SniperCrosshair->SetVisibility(ESlateVisibility::Hidden);
}

void URTSHUD::InitializeHUD()
{	
	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterPlayer();
	if (RTSCharacter)
	{
		RTSCharacter->OnWeaponTypeChanged.AddUObject(this, &URTSHUD::WeaponTypeChangedCallback);
	}

	URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponent();
	if (RTSASC)
	{
		RTSASC->RegisterGameplayTagEvent(RTSGameplayTag::FindTagByString(TEXT("Character.State.Aiming"))).AddUObject(this, &URTSHUD::IsAimingTagChanged);
	}
}

void URTSHUD::WeaponTypeChangedCallback(EWeaponType InWeaponType)
{
	CurrentWeaponType = InWeaponType;
}

void URTSHUD::IsAimingTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (CurrentWeaponType == EWeaponType::EWT_Weapon_SniperRifle)
	{
		if (NewCount > 0)
		{
			DefaultCrosshair->SetVisibility(ESlateVisibility::Hidden);
			SniperCrosshair->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DefaultCrosshair->SetVisibility(ESlateVisibility::Visible);
			SniperCrosshair->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
