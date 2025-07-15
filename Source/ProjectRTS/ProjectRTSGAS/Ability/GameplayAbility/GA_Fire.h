// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSGameplayAbility.h"
#include "GA_Fire.generated.h"

/**
 * Author		: 지용현
 * Date			: 2025.06.10
 * Description	: ULyraGameplayAbility_RangedWeapon 모방.
 *					트레이스 로직 변경(에임방향으로 한번 -> 에임방향한번, 총구에서 결과로 한번).
 *					LyraWeaponInstance로 관리하는 데이터중 필요한 것만 여기에 변수화.
 */
UCLASS()
class PROJECTRTS_API UGA_Fire : public URTSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Fire(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
protected:
	// 함수 인자에 Trace위치, 방향 넣어줄 때 구조체로 묶어 보내줌.
	struct FFiringInput
	{
		FVector TraceStart;
		FVector TraceEnd;
		FVector AimDirection;
	};

	// 로직 시작.
	UFUNCTION(BlueprintCallable)
	void StartFiring();

	// 로컬이면 Targeting 진행.
	void PerformLocalTargeting(OUT TArray<FHitResult>& OutHits);

	// Targeting Transform 구해오기 (트레이스 시작위치, 트레이스 방향(에임)).
	FTransform GetTargetingTransform(APawn* SourcePawn);
	
	// 한 번 쏠 때 정해진 탄알 수 만큼 for문으로 Trace ( 샷건을 위한 로직 ).
	void TraceBulletsInCartridge(const FFiringInput& InputData, OUT TArray<FHitResult>& OutHits);

	// 실제 트레이스 진행.
	FHitResult DoSingleBulletTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius);

	// 탄 퍼짐 계산 함수.
	FVector VRandConeNormalDistribution(const FVector& Direction, const float ConeHalfAngleRad, const float Exponent);

	// 총구 위치 얻어오기.
	const FVector GetMuzzleLocation();

	// TargetData 처리 및 콜백 등록 함수.
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	// GE, GC 실행 로직 함수.
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
	
	UFUNCTION()
	void FireComplete();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponParams")
	uint8 BulletsPerCartridge = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponParams", meta = (ForceUnits = "cm"))
	float MaxRange = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponParams", meta = (ForceUnits = "cm"))
	float BulletTraceSweepRadius = 5.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|FireParams")
	float SpreadAngle = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|FireParams", meta = (ClampMin = 0.1))
	float SpreadExponent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	uint8 bDrawDebug : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TObjectPtr<UAnimMontage> FireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	float FireDelayTime = 0.1f;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|GameplayCueTag", meta = (Categories = GameplayCue))
	// FGameplayTag GameplayCue_MuzzleEffect;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|GameplayCueTag", meta = (Categories = GameplayCue))
	// FGameplayTag GameplayCue_BulletTracer;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|GameplayCueTag", meta = (Categories = GameplayCue))
	// FGameplayTag GameplayCue_BulletImpact;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|GameplayCueTag", meta = (Categories = GameplayCue))
	FGameplayTag GameplayCue_Fire;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData|GameplayEffect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
private:
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;

	FTimerHandle FireTimerHandle;
};
