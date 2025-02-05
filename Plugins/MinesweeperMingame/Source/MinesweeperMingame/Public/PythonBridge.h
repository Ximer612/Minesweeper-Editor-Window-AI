// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PythonBridge.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPERMINGAME_API UPythonBridge : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Python)
	static UPythonBridge* Get();

	UFUNCTION(BlueprintImplementableEvent, Category = Python)
	FString FunctionImplementedInPython() const;
};
