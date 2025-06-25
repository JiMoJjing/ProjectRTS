// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "RTSAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class URTSAbilitySystemComponent;
// @Pending: 쓸지 말지는 고민해보기.
DECLARE_MULTICAST_DELEGATE_SixParams(FRTSAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float /*InCurrentHealth*/, float /*InMaxHealth*/);

/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URTSAttributeSet();

	ATTRIBUTE_ACCESSORS(URTSAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(URTSAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(URTSAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(URTSAttributeSet, Damage);

	mutable FRTSAttributeEvent OnHealthChanged;
	mutable FRTSAttributeEvent OnMaxHealthChanged;
	mutable FRTSAttributeEvent OnOutOfHealth;
	mutable FOnHPChanged OnHPChanged;

	URTSAbilitySystemComponent* GetRTSAbilitySystemComponent() const;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	// 현재 체력 어트리뷰트.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "RTS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	// 최대 체력 어트리뷰트.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "RTS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

	// Health가 0 이하가 되면 사용.
	bool bOutOfHealth;

	// 바뀌기 전 MaxHealth, Health 저장.
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	// --------------
	// MetaAttribute.
	// --------------
	UPROPERTY(BlueprintReadOnly, Category = "RTS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Category = "RTS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Damage;
};
