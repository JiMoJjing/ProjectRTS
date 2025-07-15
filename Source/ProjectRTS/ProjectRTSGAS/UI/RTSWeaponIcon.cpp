// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSWeaponIcon.h"

#include "Components/Image.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"
#include "ProjectRTS/ProjectRTSGAS/Weapon/RTSWeaponContext.h"

void URTSWeaponIcon::NativeConstruct()
{
	Super::NativeConstruct();

	uint8 WeaponTypeIndex = static_cast<uint8>(WeaponType) - 1;

	ARTSCharacterPlayer* RTSCharacter = GetRTSCharacterPlayer();
	if (!RTSCharacter)
	{
		return;
	}

	URTSWeaponContext* WeaponContext = RTSCharacter->GetWeaponContextByIndex(WeaponTypeIndex);
	if (!WeaponContext)
	{
		return;
	}

	// @Todo: WeaponContext에서 받아오기.
	UTexture* WeaponTexture = WeaponContext->WeaponIconTexture;
	if (!WeaponTexture)
	{
		return;
	}

	WeaponCard->GetDynamicMaterial()->SetTextureParameterValue(TEXT("IconTexture"), WeaponTexture);
}
