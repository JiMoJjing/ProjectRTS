// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_CharacterDamageTaken.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectRTS/ProjectRTSGAS/Feedback/DamageNiagaraComponent.h"


bool AGC_CharacterDamageTaken::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (!HitResult)
	{
		return false;
	}

	int32 DamageMagnitude = UKismetMathLibrary::Round(Parameters.RawMagnitude);

	APlayerState* PlayerState = Cast<APlayerState>(Parameters.EffectContext.GetInstigator());
	if (PlayerState)
	{
		APawn* Pawn = PlayerState->GetPawn();
		if (Pawn && Pawn->IsLocallyControlled())
		{
			AController* Controller = Pawn->GetController();
			if (Controller)
			{
				UDamageNiagaraComponent* DamageNiagaraComponent = Controller->GetComponentByClass<UDamageNiagaraComponent>();
				if (DamageNiagaraComponent)
				{
					// @Todo: 크리티컬 여부 검사하는 로직 짜기.
					bool bIsCriticalDamage = HitResult->BoneName == FName(TEXT("head"));
					
					DamageNiagaraComponent->AddDamageNiagaraEffect(HitResult->ImpactPoint, DamageMagnitude, bIsCriticalDamage);
					return true;
				}
			}
		}
	}

	return false;
}
