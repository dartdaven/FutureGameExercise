#include "HelpingTools.h"
#include <cstdarg>

void Help::DisplayDebugMessage(const FString& message)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, message);
    }
}
