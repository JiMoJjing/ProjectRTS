// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerController.h"

#include "RTSPlayerState.h"
#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Feedback/DamageNiagaraComponent.h"

ARTSPlayerController::ARTSPlayerController(const FObjectInitializer& ObjectInitializer)
{
	DamageNiagaraComponent = CreateDefaultSubobject<UDamageNiagaraComponent>(TEXT("DamageNiagaraComponent"));
}

ARTSPlayerState* ARTSPlayerController::GetRTSPlayerState() const
{
	return CastChecked<ARTSPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

URTSAbilitySystemComponent* ARTSPlayerController::GetRTSAbilitySystemComponent() const
{
	const ARTSPlayerState* RTSPlayerState = GetRTSPlayerState();
	return RTSPlayerState ? RTSPlayerState->GetRTSAbilitySystemComponent() : nullptr;
}

void ARTSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponent())
	{
		RTSASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
