// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperMingameCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperMingameModule"

void FMinesweeperMingameCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MinesweeperMingame", "Bring up MinesweeperMingame window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
