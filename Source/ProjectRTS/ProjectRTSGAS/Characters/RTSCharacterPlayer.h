// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "RTSCharacterPlayer.generated.h"

class URTSAbilitySet;
class URTSInputDataAsset;
class URTSAbilitySystemComponent;
class UGameplayAbility;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;


/**
 * Author		: 지용현
 * Date			: 2025.05.30
 * Description	: Tag로 상태를 나타낼 예정이니 상태나타내는 변수는 더 이상 사용안함.
 *					InputDataAsset관리 및 적용.
 *					AbilitySet관리 및 적용.
 *					Input Bind할 함수 생성.
 */

UCLASS()
class PROJECTRTS_API ARTSCharacterPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTSCharacterPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	//@Todo: Make GameplayAbility.
	bool TraceToCrosshair(FHitResult& OutHitResult, float InTraceDistance, ECollisionChannel InTraceChannel, bool bUseShotSpread = false);

protected:
	// SkeletalMesh SetLeaderPoseComponent.
	void SetLeaderPoseComponent();
	
	// InputBindFunction =========================================
	virtual void Input_Move(const FInputActionValue& Value);
	virtual void Input_Look(const FInputActionValue& Value);

	virtual void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	virtual void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	// ~InputBindFunction ========================================
	
	// Player Rotation Settings.
	void UseControlRotation();
	void UseMovementRotation();
	
	UFUNCTION()
	void AimingUpdate(float Alpha) const;

public:
	// Aiming Settings.
	virtual void StartAiming();
	virtual void StopAiming();

protected:
	// AbilitySet 등록.
	void RegisterAbilitySet();

protected:
	// SkeletalMesh =====================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LegMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BackMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GunMesh;
	// ~SkeletalMesh =======================================================

	
	// View ================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DefaultSpringArmLength = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingSpringArmLength = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector DefaultCameraPosition = FVector(0.0f, 160.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector AimingCameraPosition = FVector(0.0f, 80.0f, 0.0f);
	// ~View ===============================================================
	
	
	// Input ======================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URTSInputDataAsset> InputDataAsset;
	// ~Input======================================================================

	
	// GameplayAbilitySystem =============================
	UPROPERTY()
	TObjectPtr<URTSAbilitySystemComponent> RTSASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URTSAbilitySet> AbilitySet;
	// ~GameplayAbilitySystem ============================

	
	// Aiming Timeline.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> AimingTimelineCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	float AimingTimelineLength = 0.15f;

	UPROPERTY()
	FTimeline AimingTimeline;

	FOnTimelineFloat OnAimingTimelineFloat;

	// Movement Variable.
	//@Todo: Make this to AttributeSet.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 600.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BoosterSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Fire, meta = (AllowPrivateAccess = "true"))
	float MinSpreadAngle = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Fire, meta = (AllowPrivateAccess = "true"))
	float MaxSpreadAngle = 3.0f;
};