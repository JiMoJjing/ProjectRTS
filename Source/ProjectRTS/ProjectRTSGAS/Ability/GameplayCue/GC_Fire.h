// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_Fire.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRTS_API UGC_Fire : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UGC_Fire(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
