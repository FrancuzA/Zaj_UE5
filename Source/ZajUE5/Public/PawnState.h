#pragma once

#include "CoreMinimal.h"
#include "PawnState.generated.h"

UENUM(BlueprintType)
enum class EPawnState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    InCombat        UMETA(DisplayName = "InCombat"),
    Hit             UMETA(DisplayName = "Hit"),
    Exhausted       UMETA(DisplayName = "Exhausted"),
    Dead            UMETA(DisplayName = "Dead")
};