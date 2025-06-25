// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerHPBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAttributeSet.h"

void URTSPlayerHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	URTSAbilitySystemComponent* RTSASC = GetRTSAbilitySystemComponent();
	if (!RTSASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoRTSASC!"));
		return;
	}

	RTSASC->GetGameplayAttributeValueChangeDelegate(URTSAttributeSet::GetHealthAttribute()).AddUObject(this, &URTSPlayerHPBar::OnHealthChangedCallback);
	RTSASC->GetGameplayAttributeValueChangeDelegate(URTSAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &URTSPlayerHPBar::OnMaxHealthChangedCallback);

	Health = RTSASC->GetNumericAttributeBase(URTSAttributeSet::GetHealthAttribute());
	MaxHealth = RTSASC->GetNumericAttributeBase(URTSAttributeSet::GetMaxHealthAttribute());

	UpdateHP();
}

void URTSPlayerHPBar::OnHealthChangedCallback(const FOnAttributeChangeData& ChangeData)
{
	Health = ChangeData.NewValue;

	UpdateHP();
}

void URTSPlayerHPBar::OnMaxHealthChangedCallback(const FOnAttributeChangeData& ChangeData)
{
	MaxHealth = ChangeData.NewValue;

	UpdateHP();
}

void URTSPlayerHPBar::UpdateHP()
{
	const float HPRatio = Health / MaxHealth;
	HPBar->SetPercent(HPRatio);

	FString NewHPText = FString::Printf(TEXT("%d / %d"), (uint8)Health, (uint8)MaxHealth);
	HPText->SetText(FText::FromString(NewHPText));
}
