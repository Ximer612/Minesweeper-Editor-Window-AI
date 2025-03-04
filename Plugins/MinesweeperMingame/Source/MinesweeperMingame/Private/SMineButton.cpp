// Copyright 2025, Ximer - Marco Baldini, All rights reserved


#include "SMineButton.h"

void SMineButton::Construct(const FArguments& InArgs)
{
	SButton::Construct(InArgs);

	MyText = FText::FromString(DefaultStringValue);

	ChildSlot[
		SAssignNew(TextBlock,STextBlock)
			.Text(this, &SMineButton::GetText)
			.ColorAndOpacity(FColor::Red)
	];

}

void SMineButton::Press()
{
	if (!IsHidden)
	{
		return;
	}

	ClearMeAndEmptyNeighbours();
}

void SMineButton::FixValue()
{
	//a bomb dosen't need to check for near bombs since it's value doesn't change
	if (Value == INT32_MAX)
	{
		return;
	}

	int32 NearBombs = 0;

	for (size_t i = 0; i < Neighbours.Num(); i++)
	{
		if (Neighbours[i]->Value == INT32_MAX)
		{
			NearBombs++;
		}
	}

	Value = NearBombs;
}

void SMineButton::ClearMeAndEmptyNeighbours()
{
	OnPressed.ExecuteIfBound();

	if (Value == INT32_MAX)
	{
		return;
	}

	MyText = FText::FromString(FString::FromInt(Value));
	IsHidden = false;

	if (Value != 0)
	{
		return;
	}

	for (int32 i = 0; i < Neighbours.Num(); i++)
	{
		SMineButton& NeighbourBomb = Neighbours[i].Get();

		if (NeighbourBomb.IsHidden)
		{
			NeighbourBomb.ClearMeAndEmptyNeighbours();
		}
	}
}

void SMineButton::AddNeighbour(TSharedRef<SMineButton> NeighbourMine)
{
	if(Neighbours.Contains(NeighbourMine) && NeighbourMine != SharedThis(this))
	{
		return;
	}

	Neighbours.Add(NeighbourMine);
	NeighbourMine->AddNeighbour( SharedThis(this) );

	return;
}

FText SMineButton::GetText() const
{
	return MyText;
}

bool SMineButton::operator==(const SMineButton& Other)
{
	return Column == Other.Column && Row == Other.Row;
}
