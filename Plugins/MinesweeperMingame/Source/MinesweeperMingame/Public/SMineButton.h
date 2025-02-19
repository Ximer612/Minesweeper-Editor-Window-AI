// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class MINESWEEPERMINGAME_API SMineButton : public SButton
{
public:

	TSharedPtr<STextBlock> TextBlock;

	void Construct(const FArguments& InArgs);

	int32 Value;
	int32 Column;
	int32 Row;
	FString DefaultStringValue = " ";
	FText MyText;
	TArray<TSharedRef<SMineButton>> Neighbours;
	bool IsHidden = true;

	virtual void Press() override;

	void ClearMeAndEmptyNeighbours();
	void GameOver();
	void AddNeighbour(TSharedRef<SMineButton> NeighbourMine);
	FText GetText() const;

	bool operator==(const SMineButton& Other);
};
