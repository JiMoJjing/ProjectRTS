// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerController.h"

#include "RTSPlayerState.h"
#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Feedback/DamageNiagaraComponent.h"
#include "ProjectRTS/ProjectRTSGAS/UI/RTSHUD.h"

ARTSPlayerController::ARTSPlayerController(const FObjectInitializer& ObjectInitializer)
{
	DamageNiagaraComponent = CreateDefaultSubobject<UDamageNiagaraComponent>(TEXT("DamageNiagaraComponent"));

	static ConstructorHelpers::FClassFinder<URTSHUD> RTSHUDClassRef(TEXT("/Game/ProjectRTSGAS/UI/WBP_RTSHUD.WBP_RTSHUD_C"));
	if (RTSHUDClassRef.Class)
	{
		RTSHUDClass = RTSHUDClassRef.Class;
	}
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

void ARTSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void ARTSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (!IsLocalController())
	{
		return;
	}
	
	if (URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponent())
	{
		RTSASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ARTSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		RTSHUD = CreateWidget<URTSHUD>(this, RTSHUDClass, FName(TEXT("HUD")));
		if (!RTSHUD)
		{
			return;
		}
		RTSHUD->AddToViewport(0);
		RTSHUD->InitializeHUD();
	}
	
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}