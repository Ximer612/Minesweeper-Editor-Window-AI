// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once
#include "CoreMinimal.h"
#include "PythonResult.generated.h"
/**
 *
 */

USTRUCT(Blueprintable)
struct MINESWEEPERMINGAME_API FPythonResult
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Python")
	FString Response;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Python")
	FString JsonResponse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Python")
	bool bError;
};
