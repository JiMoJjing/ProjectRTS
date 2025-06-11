// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_MuzzleEffect.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECTRTS_API UGC_MuzzleEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UGC_MuzzleEffect();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue)
	FVector NiagaraScale = FVector(1.0f, 1.0f, 1.0f);
};
