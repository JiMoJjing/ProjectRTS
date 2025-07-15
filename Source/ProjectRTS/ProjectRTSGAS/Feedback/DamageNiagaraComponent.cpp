
#include "DamageNiagaraComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"

UDamageNiagaraComponent::UDamageNiagaraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DamageNiagaraSystemRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/ProjectRTSGAS/VFX/NS_DamageNumbers.NS_DamageNumbers'"));
	if (DamageNiagaraSystemRef.Object)
	{
		DamageNiagaraSystem = DamageNiagaraSystemRef.Object;
	}

	NiagaraArrayName = TEXT("DamageInfo");
	NiagaraBoolName = TEXT("Critical");
	NiagaraVec2Name = TEXT("SpriteSize");
}


void UDamageNiagaraComponent::AddDamageNiagaraEffect(const FVector& WorldLocation, const int32 Number, const bool bIsCriticalDamage)
{
	int32 LocalNumber = Number;
	FVector2D SpriteSize = FVector2D(80.0f, 50.0f);
	if (bIsCriticalDamage)
	{
		SpriteSize *= 1.6f;
	}
	
	// if (!NiagaraComponent)
	// {
	// 	if (DamageNiagaraSystem)
	// 	{
	// 		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner(), DamageNiagaraSystem, WorldLocation, FRotator::ZeroRotator, FVector(1));
	// 	}
	// 	NiagaraComponent->SetupAttachment(nullptr);
	// 	check(NiagaraComponent);
	// }

	if (DamageNiagaraSystem)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner(), DamageNiagaraSystem, WorldLocation, FRotator::ZeroRotator, FVector(1));
	}
	NiagaraComponent->SetupAttachment(nullptr);
	
	NiagaraComponent->Activate(false);
	NiagaraComponent->SetWorldLocation(WorldLocation);
	
	NiagaraComponent->SetBoolParameter(NiagaraBoolName, bIsCriticalDamage);
	NiagaraComponent->SetVariableVec2(NiagaraVec2Name, SpriteSize);
	TArray<FVector4> DamageList = UNiagaraDataInterfaceArrayFunctionLibrary::GetNiagaraArrayVector4(NiagaraComponent, NiagaraArrayName);
	DamageList.Add(FVector4(WorldLocation.X, WorldLocation.Y, WorldLocation.Z, LocalNumber));
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComponent, NiagaraArrayName, DamageList);
}
