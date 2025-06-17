

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireEffect.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class PROJECTRTS_API AFireEffect : public AActor
{
	GENERATED_BODY()

public:
	AFireEffect();
	
public:
	virtual void Fire();

protected:
	void CheckAutoDestroy();
	void ClearImpactDatas();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Impact")
	FVector MuzzlePosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Impact")
	TArray<FVector> ImpactPositions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Impact")
	TArray<FVector> ImpactNormals;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TObjectPtr<UNiagaraSystem> MuzzleNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TObjectPtr<UNiagaraSystem> TracerNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	TObjectPtr<UNiagaraSystem> ImpactNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	FName TracerNiagaraSystemArrayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS|Niagara")
	FName TracerNiagaraSystemBoolName;

protected:
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> MuzzleNiagaraComponent;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> TracerNiagaraComponent;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ImpactNiagaraComponent;
	
	FTimerHandle AutoDestroyTimerHandle;
};
