// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PythonResult.h"
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
	FPythonResult AskToAIPython(const FString& Prompt) const;

};
