

#include "RTS_EnemyBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProjectRTS/Physics/RTSCollisionChannel.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAttributeSet.h"


ARTS_EnemyBase::ARTS_EnemyBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	RTSAttributeSet = CreateDefaultSubobject<URTSAttributeSet>(TEXT("RTSAttributeSet"));

	GetCapsuleComponent()->SetCollisionProfileName(RTS_Collision_Profile_EnemyCapsule);
	GetMesh()->SetCollisionProfileName(RTS_Collision_Profile_EnemyMesh);
}

UAbilitySystemComponent* ARTS_EnemyBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARTS_EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	RTSAttributeSet->OnHealthChanged.AddUObject(this, &ARTS_EnemyBase::OnHealthChanged);
	RTSAttributeSet->OnOutOfHealth.AddUObject(this, &ARTS_EnemyBase::OnOutOfHealth);
}

void ARTS_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARTS_EnemyBase::OnHealthChanged(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldValue, float NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("GetDamage : %.1f"), EffectMagnitude);
}

void ARTS_EnemyBase::OnOutOfHealth(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldValue, float NewValue)
{
	
}
