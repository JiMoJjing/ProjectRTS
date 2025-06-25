// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSUserWidget.generated.h"

class ARTSCharacterPlayer;
class URTSAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URTSUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION(BlueprintCallable)
	URTSAbilitySystemComponent* GetRTSAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	ARTSCharacterPlayer* GetRTSCharacterPlayer();
};
