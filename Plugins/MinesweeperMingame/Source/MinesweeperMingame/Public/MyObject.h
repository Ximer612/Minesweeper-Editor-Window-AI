// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class MINESWEEPERMINGAME_API SMyObject : public SButton
{
public:

	TSharedPtr<STextBlock> TextBlock;

	void Construct(const FArguments& InArgs);

	int32 Value;
	int32 Column;
	int32 Row;
	FString DefaultStringValue = " ";
	//FText* MyText;

	virtual void Press() override;
};
