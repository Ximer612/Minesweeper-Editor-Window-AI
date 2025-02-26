// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

DECLARE_DELEGATE(FOnPressed)

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
	void FixValue();


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

private:
	void ClearMeAndEmptyNeighbours();
	void AddNeighbour(TSharedRef<SMineButton> NeighbourMine);
	FText GetText() const;

	FOnPressed OnPressed;

	friend class FMinesweeperMingameModule;
};
