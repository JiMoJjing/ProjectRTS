


#include "RTSPlayerState.h"

#include "AbilitySystemComponent.h"
#include "RTSPlayerController.h"
#include "ProjectRTS/ProjectRTS.h"
#include "ProjectRTS/ProjectRTSGAS/Ability/RTSAbilitySystemComponent.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAttributeSet.h"


ARTSPlayerState::ARTSPlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	RTSAttributeSet = CreateDefaultSubobject<URTSAttributeSet>(TEXT("RTSAttributeSet"));

	NetUpdateFrequency = 100.0f;
}

ARTSPlayerController* ARTSPlayerState::GetRTSPlayerController() const
{
	return Cast<ARTSPlayerController>(GetOwner());
}

UAbilitySystemComponent* ARTSPlayerState::GetAbilitySystemComponent() const
{
	return GetRTSAbilitySystemComponent();
}

void ARTSPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARTSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);

	// @Why: GetPawn 이 Nullptr임.
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	

	// @Pending: 무슨 로직인지 아직 모르겠다.
	// UWorld* World = GetWorld();
	// if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	// {
	// 	AGameStateBase* GameState = GetWorld()->GetGameState();
	// 	check(GameState);
	// 	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	// 	check(ExperienceComponent);
	// 	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	// }
}
