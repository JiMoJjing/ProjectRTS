// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_MuzzleEffect.h"

#include "NiagaraFunctionLibrary.h"

UGC_MuzzleEffect::UGC_MuzzleEffect()
{
}

bool UGC_MuzzleEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		const FVector SpawnLocation = HitResult->TraceStart;
		const FVector TraceEnd = HitResult->TraceEnd;
		FVector SpawnDirection = TraceEnd - SpawnLocation;
		SpawnDirection.Normalize();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraSystem, SpawnLocation, SpawnDirection.Rotation(), NiagaraScale);
		
		return true;
	}

	return false;
}
