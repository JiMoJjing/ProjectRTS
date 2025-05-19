


#include "RTSPlayerState.h"

#include "AbilitySystemComponent.h"

ARTSPlayerState::ARTSPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* ARTSPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
