// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageNiagaraComponent.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTRTS_API UDamageNiagaraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageNiagaraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION()
	void AddDamageNiagaraEffect(const FVector& WorldLocation, const int32 Number, const bool bIsCriticalDamage);

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageNiagara")
	TObjectPtr<UNiagaraSystem> DamageNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageNiagara")
	FName NiagaraArrayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageNiagara")
	FName NiagaraBoolName;
};
