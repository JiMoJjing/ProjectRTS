

#pragma once

#include "NativeGameplayTags.h"

namespace RTSGameplayTag
{	
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);

}