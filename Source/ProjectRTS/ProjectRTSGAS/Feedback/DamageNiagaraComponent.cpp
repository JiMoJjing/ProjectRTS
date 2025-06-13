
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
}


void UDamageNiagaraComponent::AddDamageNiagaraEffect(const FVector& WorldLocation, const int32 Number, const bool bIsCriticalDamage)
{
	int32 LocalNumber = Number;
	if (bIsCriticalDamage)
	{
		LocalNumber *= -1;
	}
	
	if (!NiagaraComponent)
	{
		// UE_LOG(LogTemp, Warning, TEXT("NiagaraComponent Created"));
		if (DamageNiagaraSystem)
		{
			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner(), DamageNiagaraSystem, WorldLocation, FRotator::ZeroRotator, FVector(1));
		}
		NiagaraComponent->SetupAttachment(nullptr);
		check(NiagaraComponent);
	}
	
	NiagaraComponent->Activate(false);
	NiagaraComponent->SetWorldLocation(WorldLocation);
	
	// UE_LOG(LogTemp, Warning, TEXT("NiagaraComponent Valid: %s"), NiagaraComponent ? TEXT("True") : TEXT("False"));
	// UE_LOG(LogTemp, Warning, TEXT("NiagaraSystem Valid: %s"), DamageNiagaraSystem ? TEXT("True") : TEXT("False"));
	// UE_LOG(LogTemp, Warning, TEXT("Component Location: %s"), *NiagaraComponent->GetComponentLocation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Component IsActive: %s"), NiagaraComponent->IsActive() ? TEXT("True") : TEXT("False"));
    

	
	NiagaraComponent->SetBoolParameter(NiagaraBoolName, true);
	TArray<FVector4> DamageList = UNiagaraDataInterfaceArrayFunctionLibrary::GetNiagaraArrayVector4(NiagaraComponent, NiagaraArrayName);
	DamageList.Add(FVector4(WorldLocation.X, WorldLocation.Y, WorldLocation.Z, LocalNumber * -1));
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComponent, NiagaraArrayName, DamageList);
	// UE_LOG(LogTemp, Warning, TEXT("Array Size After: %d"), DamageList.Num());
}
