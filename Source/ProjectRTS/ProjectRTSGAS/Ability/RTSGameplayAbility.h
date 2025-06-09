// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RTSGameplayAbility.generated.h"

class ARTSCharacterPlayer;
class ARTSPlayerController;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class URTSAbilitySystemComponent;

public:
	URTSGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	URTSAbilitySystemComponent* GetRTSAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	ARTSPlayerController* GetRTSPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	ARTSCharacterPlayer* GetRTSCharacterFromActorInfo() const;

protected:
	// UGameplayAbility override.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	
};
