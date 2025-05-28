// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

class URTSAbilitySystemComponent;
class ARTSPlayerState;
/**
 * Author		: 지용현
 * Date			: 2025.05.28
 * Description	: 입력 일괄 처리를 위해 PostProcessInput 오버라이드.
 */
UCLASS()
class PROJECTRTS_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "RTS|PlayerController")
	ARTSPlayerState* GetRTSPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "RTS|PlayerController")
	URTSAbilitySystemComponent* GetRTSAbilitySystemComponent() const;

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
