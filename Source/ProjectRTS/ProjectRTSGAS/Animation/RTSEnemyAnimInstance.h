// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RTSEnemyAnimInstance.generated.h"

class ARTS_EnemyBase;
enum class EEnemyState : uint8;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	void OnEnemyStateChangedCallback(EEnemyState NewState);
	
public:
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState;

};
