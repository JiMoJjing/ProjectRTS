// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);
/**
 * Author		: 지용현
 * Date			: 2025.05.19
 * Description	: RTSGA_Jump에서 생성해서 사용하는 AbilityTask이다.
 *					CreateTask에서 자신을 생성해서 리턴한다.
 *					Activate에서 캐릭터의 LandedDelegate에 Callback을 바인드하고
 *					캐릭터의 Jump 함수를 실행한다. SetWaitingOnAvatar를 호출하고 Activate를 마무리한다.
 *					OnDestory 호출될 때 캐릭터의 LandedDelegate에 바인드했던 함수를 제거한다.
 *					OnLandedCallback에서 RTSGA_Jump에게 이 AbilityTask가 끝났음을 알린다.
 */
UCLASS()
class PROJECTRTS_API UAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_JumpAndWaitForLanding();
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="JumpAndWaitForLanding",
	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	FJumpAndWaitForLandingDelegate OnCompleted;

protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& LandHit);
};
