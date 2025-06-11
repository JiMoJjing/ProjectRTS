// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_BulletTracer.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UGC_BulletTracer::UGC_BulletTracer()
{
	
}

bool UGC_BulletTracer::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult && NiagaraSystem)
	{
		const FVector TracerStart = HitResult->TraceStart;
		const FVector TracerEnd = HitResult->bBlockingHit ? HitResult->ImpactPoint : HitResult->TraceEnd;
		
		UNiagaraComponent* BulletTracer = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, NiagaraSystem, TracerStart, FRotator::ZeroRotator);
		BulletTracer->SetVectorParameter(VectorParameterName, TracerEnd);
		return true;
	}	
	return false;
}
