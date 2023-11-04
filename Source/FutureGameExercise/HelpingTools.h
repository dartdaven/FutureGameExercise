#pragma once

#include "CoreMinimal.h"

namespace Help
{
	//void DisplayDebugMessage(const TCHAR* message, ...);

	template <typename FmtType, typename... Types>
	void DisplayDebugMessage(const FmtType& Fmt, Types... Args)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(Fmt, Args...));
		}
	}
}

#define DISPLAY_DEBUG_MESSAGE(text, ...) \
	do { \
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT(text), ##__VA_ARGS__)); \
	} while (0)
