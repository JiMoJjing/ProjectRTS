// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "RTS_EnemyBase.generated.h"

class UGameplayEffect;
class UFloatingHPBarComponent;
class URTSAttributeSet;
struct FGameplayEffectSpec;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Idle UMETA(DisplayName = "Idle"),
	EES_Dead UMETA(DisplayName = "Dead")
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyStateChanged, EEnemyState);

UCLASS()
class PROJECTRTS_API ARTS_EnemyBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTS_EnemyBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	EEnemyState GetEnemyState() const { return EnemyState; }

	FOnEnemyStateChanged OnEnemyStateChanged;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void SetEnemyState(EEnemyState NewState);
	void OnHPChangedCallback(float InCurrentHealth, float InMaxHealth);
	void OnOutOfHealth();

	UFUNCTION()
	void OnRep_EnemyState();


protected:
	UPROPERTY(EditAnywhere, Category = "RTS|GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "RTS|GAS")
	TSubclassOf<UGameplayEffect> InitStatEffectClass;

	UPROPERTY()
	TObjectPtr<URTSAttributeSet> RTSAttributeSet;

	UPROPERTY(EditAnywhere, Category = "RTS|Widget")
	TObjectPtr<UFloatingHPBarComponent> FloatingHPBarComponent;
	
	UPROPERTY(ReplicatedUsing = OnRep_EnemyState)
	EEnemyState EnemyState = EEnemyState::EES_Idle;
};