// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSUserWidget.h"
#include "RTSWeaponIcon.generated.h"

enum class EWeaponType : uint8;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSWeaponIcon : public URTSUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> WeaponCard;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;
};
