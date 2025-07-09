
#include "TransitionEvents.h"



bool UTransitionEvents::TransitionCondition_Implementation(AActor* StateContext, const FGameplayTag& InputAction,
	const FInputActionValue& Value, const UStateBase* CurrentState, const FGameplayTag& NewState)
{
	return true;
}