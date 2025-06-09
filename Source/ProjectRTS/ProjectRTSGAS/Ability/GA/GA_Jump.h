// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSGameplayAbility.h"
#include "GA_Jump.generated.h"

/**
 * Author		: 지용현
 * Date			: 2025.05.19
 * Description	: 캐릭터의 점프를 실행하는 GameplayAbility이다.
 *					CanActivateAbility 에서 캐릭터가 점프 가능한 상태인지 체크해서 Ability 실행 가능 여부를 검사한다.
 *					ActivateAbility 에서 URTSAT_JumpAndWaitForLanding를 사용하여 실질적인 동작을 실행한다.
 *					URTSAT_JumpAndWaitForLanding에 선언된 DELEGATE를 통해 콜백을 받아 EndAbility를 한다.
 * Date			: 2025.06.09
 * Description	: URTSGameplayAbility 상속받도록 변경.
 *					AT_Jump 빼고 UAbilityTask_StartAbilityState, UAbilityTask_WaitInputRelease 쓰도록 변경.
 *					로컬에서만 점프 실행되도록 구현.
 */
UCLASS()
class PROJECTRTS_API UGA_Jump : public URTSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 점프의 사용 가능 조건 설정.
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	// 점프 실행.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	void OnWaitInputReleasedCallback(float TimeHeld);

	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "RTS|Ability")
	void CharacterJumpStop();
};
