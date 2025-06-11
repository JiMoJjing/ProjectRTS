// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_BulletImpact.h"

#include "NiagaraFunctionLibrary.h"

UGC_BulletImpact::UGC_BulletImpact()
{
}

bool UGC_BulletImpact::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult && NiagaraSystem && HitResult->bBlockingHit)		
	{
		const FVector ImpactLocation = HitResult->ImpactPoint;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraSystem, ImpactLocation, FRotator::ZeroRotator);

		return true;
	}
	
	return false;
}
