// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RTSPlayerAniminstance.generated.h"

class UCharacterMovementComponent;
class ARTSPlayerBase;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSPlayerAniminstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URTSPlayerAniminstance();
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Caching)
	TObjectPtr<ARTSPlayerBase> RTSPlayerBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Caching)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Velocity = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint32 bIsMove : 1 = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint32 bIsFalling : 1 = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint32 bIsAiming : 1 = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint32 bIsBooster : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Direction)
	float Direction = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Direction)
	float Pitch = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Direction)
	float BoosterDirection = 0.0f;
	
};
