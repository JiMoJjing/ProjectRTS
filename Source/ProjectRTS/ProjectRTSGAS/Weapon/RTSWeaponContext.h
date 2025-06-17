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
	
};
