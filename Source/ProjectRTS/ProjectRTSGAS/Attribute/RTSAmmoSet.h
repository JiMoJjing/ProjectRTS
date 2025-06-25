// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "RTSAmmoSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, float /*CurrentAmmo*/, float /*MaxAmmo*/);

/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSAmmoSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URTSAmmoSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATTRIBUTE_ACCESSORS(URTSAmmoSet, CurrentAmmo);
	ATTRIBUTE_ACCESSORS(URTSAmmoSet, MaxAmmo);

	mutable FOnAmmoChanged OnAmmoChanged;

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
private:
	UPROPERTY(BlueprintReadOnly, Category = "RTS|Ammo", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CurrentAmmo;

	UPROPERTY(BlueprintReadOnly, Category = "RTS|Ammo", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxAmmo;
};
