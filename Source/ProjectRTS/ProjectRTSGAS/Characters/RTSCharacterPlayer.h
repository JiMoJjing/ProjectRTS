// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "RTSCharacterPlayer.generated.h"

class UGameplayAbility;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTRTS_API ARTSCharacterPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTSCharacterPlayer();
	
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetupGASInputComponent();

	//@Todo: These may be GameplayTag.
	FORCEINLINE bool IsAiming() const { return bIsAiming; }
	FORCEINLINE bool IsBooster() const { return bIsBooster; }

	//@Todo: This will be created to TargetActor.
	bool TraceToCrosshair(FHitResult& OutHitResult, float InTraceDistance, ECollisionChannel InTraceChannel, bool bUseShotSpread = false);

protected:
	// SkeletalMesh SetLeaderPoseComponent.
	void SetLeaderPoseComponent();
	
	// Input Bind Functions.
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);

	// GAS Input.
	virtual void GASInputPressed(int32 InputID);
	virtual void GASInputReleased(int32 InputID);
	
	// Player Rotation Settings.
	void UseControlRotation();
	void UseMovementRotation();

	// Aiming Settings.
	virtual void BeginAimingSettings();
	virtual void StopAimingSettings();

	UFUNCTION()
	void AimingUpdate(float Alpha) const;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	// Modular Part ( SkeletalMeshComponent ) - Body(Character Mesh), Arm, Head, Leg, Back, Gun.
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

	// SpringArm, Camera Component ans Variables.
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

	// Enhanced Input.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftMouseInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightMouseInputAction;

	// GameplayAbilities.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TMap<int32, TSubclassOf<UGameplayAbility>> InputAbilities;

	// PlayerCharacter behavior flag bool variables.
	//@Todo: Make this GameplayTag.
	UPROPERTY(BlueprintReadOnly)
	uint32 bIsAiming : 1 = false;
	
	UPROPERTY(BlueprintReadOnly)
	uint32 bIsBooster : 1 = false;
	
	// Aiming Timeline.
	//@Todo: Use GameplayTag Delegate for Aiming.
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