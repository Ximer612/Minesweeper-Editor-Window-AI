// Fill out your copyright notice in the Description page of Project Settings.

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
};
