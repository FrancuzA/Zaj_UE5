// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZajUE5GameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AZajUE5GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AZajUE5GameMode();
};



