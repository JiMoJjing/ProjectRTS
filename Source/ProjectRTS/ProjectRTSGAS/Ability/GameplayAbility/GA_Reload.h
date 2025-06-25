// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSGameplayAbility.h"
#include "GA_Reload.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRTS_API UGA_Reload : public URTSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Reload(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> ReloadEffectClass;
	
	UFUNCTION()
	void ReloadCancelled();

	UFUNCTION()
	void ReloadSuccess();
};
