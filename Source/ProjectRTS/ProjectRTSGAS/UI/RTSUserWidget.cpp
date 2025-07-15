// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUserWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Characters/RTSCharacterPlayer.h"

URTSUserWidget::URTSUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

URTSAbilitySystemComponent* URTSUserWidget::GetRTSAbilitySystemComponent()
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		RTS_UILOG(LogRTS, Log, TEXT("OwningPawn is nullptr!"));
		return nullptr;
	}
	
	return Cast<URTSAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn));
}

ARTSCharacterPlayer* URTSUserWidget::GetRTSCharacterPlayer()
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		RTS_UILOG(LogRTS, Log, TEXT("OwningPawn is nullptr!"));
		return nullptr;
	}

	return Cast<ARTSCharacterPlayer>(OwningPawn);
}
