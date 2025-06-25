// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSUserWidget.h"
#include "RTSPlayerHPBar.generated.h"

struct FOnAttributeChangeData;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSPlayerHPBar : public URTSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	void OnHealthChangedCallback(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthChangedCallback(const FOnAttributeChangeData& ChangeData);

	void UpdateHP();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;

	float Health = 0.0f;
	float MaxHealth = 0.0f;
};

