// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "ProjectRTS/ProjectRTSGAS/Weapon/RTSWeaponContext.h"
#include "RTSCharacterPlayer.generated.h"

class AFireEffect;
class URTSWeaponContext;
class URTSAbilitySet;
class URTSInputContext;
class URTSAbilitySystemComponent;
class UGameplayAbility;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, EWeaponType);

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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void InitializeWeapon();

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

protected:
	// SkeletalMesh SetLeaderPoseComponent.
	void SetLeaderPoseComponent();
	
	// InputBindFunction =========================================
	virtual void Input_Move(const FInputActionValue& Value);
	virtual void Input_Look(const FInputActionValue& Value);

	virtual void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	virtual void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	virtual void Input_WeaponSwap(uint8 WeaponIndex);
	// ~InputBindFunction ========================================
	
	
	UFUNCTION()
	void AimingUpdate(float Alpha) const;

	UFUNCTION()
	void RecoilUpdate(float Alpha);

	UFUNCTION()
	void RecoilFinish();

public:
	// Player Rotation Settings.
	void UseControlRotation();
	void UseMovementRotation();

	void SetWalkSpeed();
	void SetRunSpeed();

	// Aiming Settings.
	void StartAiming();
	void StopAiming();

	FVector GetMuzzleSocketLocation() const;
	
	void SpawnFireEffectActor(TArray<FVector>& InImpactPositions, TArray<FVector>& InImpactNormals);

	FVector VRandConeNormalDistribution(const FVector& Direction, const float ConeHalfAngleRad, const float Exponent);

	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

	void StartFireRecoil();
	
protected:
	// AbilitySet 등록.
	void RegisterAbilitySet();

public:
	void WeaponSwapAbilitySuccess(uint8 InWeaponIndex);
	
	UFUNCTION()
	void OnRep_CurrentWeaponType();

	URTSWeaponContext* GetCurrentWeaponContext() const { return CurrentWeaponContext; }
	URTSWeaponContext* GetWeaponContextByIndex(uint8 Index) const;

	void OnAmmoChangedCallback(float InCurrentAmmo, float InMaxAmmo);

	FOnWeaponTypeChanged OnWeaponTypeChanged;

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
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	// ~SkeletalMesh =======================================================

	
	// View ================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DefaultFOV = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingFOV = 75.0f;

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
	TObjectPtr<URTSInputContext> InputDataAsset;
	// ~Input======================================================================

	
	// GameplayAbilitySystem =============================
	UPROPERTY()
	TObjectPtr<URTSAbilitySystemComponent> RTSASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URTSAbilitySet> AbilitySet;
	// ~GameplayAbilitySystem ============================

	
	// Aiming Timeline ==============================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> AimingTimelineCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	float AimingTimelineLength = 0.15f;

	UPROPERTY()
	FTimeline AimingTimeline;

	FOnTimelineFloat OnAimingTimelineFloat;
	// ~Aiming Timeline =============================

	// Recoil Timeline ==============================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> RecoilTimelineCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	float RecoilTimelineLength = 0.15f;

	UPROPERTY()
	FTimeline RecoilTimeline;

	FOnTimelineFloat OnRecoilTimelineFloat;

	UPROPERTY()
	float RecoilAddYaw = 0.0f;
	
	UPROPERTY()
	float RecoilAddPitch = 0.0f;
	// ~Recoil Timeline =============================

	// Movement Variable.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 600.0f;

	// GameplayTag Blueprint Property Map
	UPROPERTY(EditAnywhere, Category = "RTS|GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	// WeaponContext.
	UPROPERTY()
	TObjectPtr<URTSWeaponContext> CurrentWeaponContext;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentWeaponType)
	EWeaponType CurrentWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|WeaponContext")
	TMap<uint8, TObjectPtr<URTSWeaponContext>> WeaponContexts;
	
	UPROPERTY()
	TObjectPtr<AFireEffect> FireEffectActor;

	// Reload.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Reload", meta = (Categories = "InputTag"))
	FGameplayTag ReloadInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|WeaponSwap", meta = (Categories = "InputTag"))
	FGameplayTag WeaponSwapInputTag;
};