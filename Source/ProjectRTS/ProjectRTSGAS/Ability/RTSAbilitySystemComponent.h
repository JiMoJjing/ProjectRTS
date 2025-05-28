

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RTSAbilitySystemComponent.generated.h"

/**
 * Author		: 지용현
 * Date			: 2025.05.28
 * Description	: 기능 확장된 AbilitySystemComponent.
 *					Ability입력을 Tag를 받아서 처리하고 즉각 처리가 아닌 Tick마다 일괄 처리.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTRTS_API URTSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	URTSAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Tag받아서 입력 처리.
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// Tick마다 Pressed, Released, Held 처리.
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	// @Pending: 현재 필요하지 않은 오버라이딩.
	// virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

protected:
	// Spec기반 Ability Input처리.
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
