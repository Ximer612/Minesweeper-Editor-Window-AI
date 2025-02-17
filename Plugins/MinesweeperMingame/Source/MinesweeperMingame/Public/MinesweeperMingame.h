// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class SMyObject;

class FMinesweeperMingameModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	FReply SendPrompt(bool bResendLast = false);

	void SendLastPrompt(const FText& InText, ETextCommit::Type CommitType);
	void ClearMinesweeperMinigame();
	void AddButtonMinesweeperMinigame(const FString& InString, const int32& InColumn, const int32& InRow);
	void AddTextBlockToScrollBox(const FText& InText, const FSlateColor& InColor);

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<SEditableTextBox> SendPromptEditableTextBox;
	TSharedPtr<SScrollBox> ChatScrollBox;
	TSharedPtr<SGridPanel> MinesGridPanel;

	TArray<TSharedRef<SMyObject>> MinesButtons;
	TArray<int32> MinesweeperField;
	int32 MinesweeperMines;

	FText PromptToSend;
	bool bIsAiThinking;
	FOnClicked OnClickedOnMinesweeperButton;

};
