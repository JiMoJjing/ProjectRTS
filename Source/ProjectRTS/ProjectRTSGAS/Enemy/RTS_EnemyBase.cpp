

#include "RTS_EnemyBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectRTS/Physics/RTSCollisionChannel.h"
#include "ProjectRTS/ProjectRTSGAS/Attribute/RTSAttributeSet.h"
#include "ProjectRTS/ProjectRTSGAS/Components/FloatingHPBarComponent.h"


ARTS_EnemyBase::ARTS_EnemyBase()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);

	RTSAttributeSet = CreateDefaultSubobject<URTSAttributeSet>(TEXT("RTSAttributeSet"));
	FloatingHPBarComponent = CreateDefaultSubobject<UFloatingHPBarComponent>(TEXT("FloatingHPBarComponent"));
	FloatingHPBarComponent->SetIsReplicated(true);
	FloatingHPBarComponent->SetupAttachment(GetMesh());
	FloatingHPBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	GetCapsuleComponent()->SetCollisionProfileName(RTS_Collision_Profile_EnemyCapsule);
	GetMesh()->SetCollisionProfileName(RTS_Collision_Profile_EnemyMesh);
}

UAbilitySystemComponent* ARTS_EnemyBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void ARTS_EnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARTS_EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	const URTSAttributeSet* RTSSet = ASC->GetSet<URTSAttributeSet>();
	RTSSet->OnHPChanged.AddUObject(this, &ARTS_EnemyBase::OnHPChangedCallback);

	if (InitStatEffectClass)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffectClass, 1.0f, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}

	OnHPChangedCallback(RTSSet->GetHealth(), RTSSet->GetMaxHealth());
}

void ARTS_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARTS_EnemyBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTS_EnemyBase, EnemyState);
}

void ARTS_EnemyBase::SetEnemyState(EEnemyState NewState)
{
	EnemyState = NewState;
	OnRep_EnemyState();
}

void ARTS_EnemyBase::OnHPChangedCallback(float InCurrentHealth, float InMaxHealth)
{
	FloatingHPBarComponent->UpdateHPBar(InCurrentHealth, InMaxHealth);
	if (InCurrentHealth <= 0.0f)
	{
		OnOutOfHealth();
	}
}

void ARTS_EnemyBase::OnOutOfHealth()
{
	SetEnemyState(EEnemyState::EES_Dead);
}

void ARTS_EnemyBase::OnRep_EnemyState()
{
	OnEnemyStateChanged.Broadcast(EnemyState);

	if (EnemyState == EEnemyState::EES_Dead)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FTimerHandle DeadTimerHandle;
		GetWorldTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetVisibility(false);
		}), 5.0f, false);
	}
}
