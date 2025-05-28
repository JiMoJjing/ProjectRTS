

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RTSPlayerState.generated.h"

class URTSAbilitySystemComponent;
class ARTSPlayerController;
/**
 * Author		: 지용현
 * Date			: 2025.05.19
 * Description	: 플레이어의 ASC 관리 주체.
 *					AttributeSet 도 여기에 추가할 예정.
 *					
 * Update		: 2025.05.28
 * Description	: RTSAbilitySystemComponent 생성 및 관리.
 */
UCLASS()
class PROJECTRTS_API ARTSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTSPlayerState(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "RTS|PlayerState")
	ARTSPlayerController* GetRTSPlayerController() const;

	URTSAbilitySystemComponent* GetRTSAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<URTSAbilitySystemComponent> AbilitySystemComponent;
};
