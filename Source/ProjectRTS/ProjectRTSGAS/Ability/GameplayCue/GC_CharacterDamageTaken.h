// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_BurstLatent.h"
#include "GC_CharacterDamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRTS_API AGC_CharacterDamageTaken : public AGameplayCueNotify_BurstLatent
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};
