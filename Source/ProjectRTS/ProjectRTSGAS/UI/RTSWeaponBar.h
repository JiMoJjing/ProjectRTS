// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSUserWidget.h"
#include "RTSWeaponBar.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSWeaponBar : public URTSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	void OnAmmoChanged(float InCurrentAmmo, float InMaxAmmo);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText;
};
