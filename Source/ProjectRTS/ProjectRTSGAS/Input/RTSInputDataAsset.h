// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RTSInputDataAsset.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FRTSInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag;
};

/**
 * Author		: 지용현
 * Date			: 2025.05.28
 * Description	: InputAction과 Tag를 묶어서 관리하는 데이터 애셋.
 */
UCLASS()
class PROJECTRTS_API URTSInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URTSInputDataAsset(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "RTS|Pawn")
	const UInputAction* FindNativeActionForTag(const FGameplayTag& InputTag) const;

	UFUNCTION(BlueprintCallable, Category = "RTS|Pawn")
	const UInputAction* FindAbilityActionForTag(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FRTSInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FRTSInputAction> AbilityInputActions;
};
