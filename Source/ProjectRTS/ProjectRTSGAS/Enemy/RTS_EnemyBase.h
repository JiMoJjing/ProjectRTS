// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "RTS_EnemyBase.generated.h"

struct FGameplayEffectSpec;
class URTSAttributeSet;

UCLASS()
class PROJECTRTS_API ARTS_EnemyBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTS_EnemyBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<URTSAttributeSet> RTSAttributeSet;

protected:
	void OnHealthChanged(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldValue, float NewValue);
	
	void OnOutOfHealth(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldValue, float NewValue);
};
