#pragma once

// If Unreal Engine headers are available, include the real headers so UHT/UE code works.
// Otherwise provide small stubs so external tools / IDE parsers don't error when engine headers
// are not on the include path.

#if defined(__has_include)
# if __has_include("CoreMinimal.h")
# include "CoreMinimal.h"
# include "UObject/Interface.h"
# include "InteractionInterface.generated.h"

// Klasa reprezentująca interfejs w systemie Unreal (wymagana)
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

// Czysty interfejs – implementowany przez klasy aktorów
class ZAJUE5_API IInteractionInterface
{
	GENERATED_BODY()

public:
	// Metoda do zaimplementowania przez każdy interaktywny obiekt
	virtual void Interact(class ABasePlayerCharacter* Interactor) =0;
};

# else
// Fallback stubs for environments where Unreal headers aren't available (editor parsers, intellisense)

// Minimal UInterface stub
class UInterface {};
#define UINTERFACE(...)
#define GENERATED_BODY()
#define MinimalAPI
#ifndef ZAJUE5_API
# define ZAJUE5_API
#endif

class UInteractionInterface : public UInterface
{
};

class ABasePlayerCharacter;

class IInteractionInterface
{
public:
	virtual void Interact(ABasePlayerCharacter* Interactor) =0;
};

# endif
#else
// If __has_include is not supported, try to include UE headers directly; if that fails the build will show proper errors.
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class ZAJUE5_API IInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(class ABasePlayerCharacter* Interactor) =0;
};
#endif
