// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "Framework/Commands/Commands.h"
#include "MinesweeperMingameStyle.h"

class FMinesweeperMingameCommands : public TCommands<FMinesweeperMingameCommands>
{
public:

	FMinesweeperMingameCommands()
		: TCommands<FMinesweeperMingameCommands>(TEXT("MinesweeperMingame"), NSLOCTEXT("Contexts", "MinesweeperMingame", "MinesweeperMingame Plugin"), NAME_None, FMinesweeperMingameStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};