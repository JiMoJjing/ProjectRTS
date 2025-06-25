

#include "RTSEnemyAnimInstance.h"

#include "ProjectRTS/ProjectRTSGAS/Enemy/RTS_EnemyBase.h"

void URTSEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ARTS_EnemyBase* EnemyBaseRef = Cast<ARTS_EnemyBase>(GetOwningActor());
	if (EnemyBaseRef)
	{
		EnemyBaseRef->OnEnemyStateChanged.AddUObject(this, &URTSEnemyAnimInstance::OnEnemyStateChangedCallback);
	}
}

void URTSEnemyAnimInstance::OnEnemyStateChangedCallback(EEnemyState NewState)
{
	EnemyState = NewState;
}
