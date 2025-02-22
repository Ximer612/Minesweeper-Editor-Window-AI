// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PythonResult.h"
#include "PythonBridge.generated.h"

DECLARE_DELEGATE(FOnUpdatedText)

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

	UFUNCTION(BlueprintCallable, Category = Python)
	void UpdateTextBlock(const FString& NewString) const;


public:

	UPROPERTY(BlueprintReadWrite, Category = Python)
	FString AIModel = "unrealAI:latest";

	UPROPERTY(BlueprintReadWrite, Category = Python)
	FString AIUrl = "http://localhost:11434/api/generate";

	UPROPERTY(BlueprintReadWrite, Category = Python)
	bool bAIStream = true;

	UPROPERTY(BlueprintReadWrite,Category = Python)
	bool bIsAiThinking;

//private:
	TSharedPtr<STextBlock> ChatTextBlock;

	FOnUpdatedText OnUpdatedText;
};
