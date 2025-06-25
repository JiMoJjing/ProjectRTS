// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSUserWidget.h"
#include "RTSHUD.generated.h"

struct FGameplayTag;
enum class EWeaponType : uint8;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSHUD : public URTSUserWidget
{
	GENERATED_BODY()

public:
	URTSHUD(const FObjectInitializer& ObjectInitializer);

	void InitializeHUD();

protected:
	virtual void NativeConstruct() override;

	void WeaponTypeChangedCallback(EWeaponType InWeaponType);

	void IsAimingTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> DefaultCrosshair;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> SniperCrosshair;

	UPROPERTY(BlueprintReadOnly)
	EWeaponType CurrentWeaponType;
};
