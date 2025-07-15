// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RTSWeaponContext.generated.h"

class AFireEffect;
class UNiagaraSystem;
class UGameplayAbility;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Weapon_None = 0 UMETA(DisplayName = "None", Hidden),
	EWT_Weapon_AssaultRifle = 1 UMETA(DisplayName = "AssaultRifle"),
	EWT_Weapon_Shotgun = 2 UMETA(DisplayName = "Shotgun"),
	EWT_Weapon_SniperRifle = 3 UMETA(DisplayName = "SniperRifle"),
	EWT_Weapon_Pistol = 4 UMETA(DisplayName = "Pistol"),
	EWT_Weapon_Max = 5 UMETA(DisplayName = "Max", Hidden)
};

USTRUCT(BlueprintType)
struct FFireRecoil
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float YawMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float YawMax = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PitchMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PitchMax = 0.0f;
};

/**
 * 
 */
UCLASS()
class PROJECTRTS_API URTSWeaponContext : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URTSWeaponContext(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Weapon")
	uint8 MaxAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Weapon")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Weapon")
	float WeaponDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	float DefaultFOV = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	float AimingFOV = 75.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	FVector DefaultCameraPosition = FVector(0.0f, 160.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	FVector AimingCameraPosition = FVector(0.0f, 80.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	float DefaultSpringArmLength = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	float AimingSpringArmLength = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Camera")
	FFireRecoil FireRecoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Mesh")
	TObjectPtr<USkeletalMesh> WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Mesh")
	FName MuzzleSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|GameplayAbility")
	TSubclassOf<UGameplayAbility> WeaponGA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|GameplayAbility", meta = (Categories = InputTag))
	FGameplayTag WeaponInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Type")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	uint8 bNeedFakeData : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara", meta = (EditCondition = bNeedFakeData))
	uint8 NumberOfFakeData = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TObjectPtr<UNiagaraSystem> MuzzleNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TObjectPtr<UNiagaraSystem> TracerNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TObjectPtr<UNiagaraSystem> ImpactNiagara;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TSubclassOf<AFireEffect> FireEffectActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Texture")
	TObjectPtr<UTexture> WeaponIconTexture;
	
};
