#pragma once

#include "CoreMinimal.h"

namespace Help
{
	void DisplayDebugMessage(const FString& message);
}

#define DISPLAY_DEBUG_MESSAGE(text, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT(text), ##__VA_ARGS__));
