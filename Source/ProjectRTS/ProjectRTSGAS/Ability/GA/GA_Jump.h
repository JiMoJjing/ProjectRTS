// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Jump.generated.h"

/**
 * Author		: 지용현
 * Date			: 2025.05.19
 * Description	: 캐릭터의 점프를 실행하는 GameplayAbility이다.
 *					CanActivateAbility 에서 캐릭터가 점프 가능한 상태인지 체크해서 Ability 실행 가능 여부를 검사한다.
 *					ActivateAbility 에서 URTSAT_JumpAndWaitForLanding를 사용하여 실질적인 동작을 실행한다.
 *					URTSAT_JumpAndWaitForLanding에 선언된 DELEGATE를 통해 콜백을 받아 EndAbility를 한다.
 */
UCLASS()
class PROJECTRTS_API UGA_Jump : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Jump();

	// 점프의 사용 가능 조건 설정.
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	// 점프 실행.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 점프 입력 Released 됐을 때.
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	// AbilityTask가 종료될 때 콜백 받는 함수. EndAbility를 실행한다.
	UFUNCTION()
	void OnLandedCallback();
};
