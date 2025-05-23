

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RTSPlayerState.generated.h"

/**
 * Author		: 지용현
 * Date			: 2025.05.19
 * Description	: 플레이어의 ASC 관리 주체.
 *					AttributeSet 도 여기에 추가할 예정.
 *					
 */
UCLASS()
class PROJECTRTS_API ARTSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTSPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;
};
