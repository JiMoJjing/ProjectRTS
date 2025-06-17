

#include "FireEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"


AFireEffect::AFireEffect()
{
	TracerNiagaraSystemArrayName = FName(TEXT("User.ImpactPositions"));
	TracerNiagaraSystemBoolName = FName(TEXT("User.Trigger"));
}

void AFireEffect::Fire()
{
	check(MuzzleNiagaraSystem);
	check(TracerNiagaraSystem);
	check(ImpactNiagaraSystem);
	
	// Muzzle.
	FVector MuzzleDirection = ImpactPositions[0] - MuzzlePosition;
	MuzzleDirection.Normalize();
	
	MuzzleNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleNiagaraSystem, MuzzlePosition, MuzzleDirection.Rotation(), FVector(2.0f, 2.0f, 2.0f));

	// Tracer.
	TracerNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, TracerNiagaraSystem, MuzzlePosition, FRotator::ZeroRotator);
	TracerNiagaraComponent->SetVariableBool(TracerNiagaraSystemBoolName, true);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(TracerNiagaraComponent, TracerNiagaraSystemArrayName, ImpactPositions);
	

	// Impact.
	for (uint8 ix = 0; ix < ImpactPositions.Num(); ++ix)
	{
		if (!ImpactNormals.IsValidIndex(ix))
		{
			continue;
		}
		ImpactNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagaraSystem, ImpactPositions[ix], ImpactNormals[ix].Rotation());
	}

	// Array 리셋.
	ClearImpactDatas();

	// 3초 동안 재활용 안하면 자동 소멸.
	CheckAutoDestroy();
}

void AFireEffect::CheckAutoDestroy()
{
	if (GetWorldTimerManager().IsTimerActive(AutoDestroyTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(AutoDestroyTimerHandle);
	}

	GetWorldTimerManager().SetTimer(AutoDestroyTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		bool Flag = false;
		Flag |= IsValid(MuzzleNiagaraComponent);
		Flag |= IsValid(TracerNiagaraComponent);
		Flag |= IsValid(ImpactNiagaraComponent);

		if (Flag)
		{
			CheckAutoDestroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FireEffect Destroyed!"));
			Destroy();
		}		
	}), 3.0f, false);
}

void AFireEffect::ClearImpactDatas()
{
	ImpactPositions.Empty();
	ImpactNormals.Empty();
}
