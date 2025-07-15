// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSGameplayAbility.h"
#include "GA_WeaponSwap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRTS_API UGA_WeaponSwap : public URTSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_WeaponSwap(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void SwapCancelled();
	
	UFUNCTION()
	void SwapFinished();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Montage")
	TObjectPtr<UAnimMontage> WeaponSwapMontage;

	uint8 WeaponIndex = 0;
};