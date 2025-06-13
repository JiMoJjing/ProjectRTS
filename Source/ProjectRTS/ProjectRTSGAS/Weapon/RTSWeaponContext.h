// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RTSWeaponContext.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	TSubclassOf<UGameplayAbility> WeaponGA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS", meta = (Categories = InputTag))
	FGameplayTag WeaponInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	EWeaponType WeaponType;
};
