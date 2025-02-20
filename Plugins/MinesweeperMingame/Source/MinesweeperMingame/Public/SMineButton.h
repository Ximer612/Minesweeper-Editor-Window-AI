// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

DECLARE_DELEGATE(FOnGameOver)

#include "SlateBasics.h"
#include "SlateExtras.h"


/**
 * 
 */
class MINESWEEPERMINGAME_API SMineButton : public SButton
{
public:

	void Construct(const FArguments& InArgs);
	virtual void Press() override;

	bool operator==(const SMineButton& Other);

private:
	TSharedPtr<STextBlock> TextBlock;

	int32 Value;
	int32 Column;
	int32 Row;
	FString DefaultStringValue = " ";
	FText MyText;
	TArray<TSharedRef<SMineButton>> Neighbours;
	bool IsHidden = true;

	void ClearMeAndEmptyNeighbours();
	void GameOver();
	void AddNeighbour(TSharedRef<SMineButton> NeighbourMine);
	FText GetText() const;

	FOnGameOver OnGameOver;

	friend class FMinesweeperMingameModule;
};
