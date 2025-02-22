// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class SMineButton;
class UPythonBridge;

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
	FReply ClearMinesweeperMinigame();
	void AddButtonMinesweeperMinigame(const FString& InString, const int32& InColumn, const int32& InRow);
	void MinesweeperGameOver();
	void ScrollToEndChatBox();
	TSharedRef<STextBlock> AddTextBlockToScrollBox(const FString& InString, const FSlateColor& InColor, const FString& SpeakerName);

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<SEditableTextBox> SendPromptEditableTextBox;
	TSharedPtr<SScrollBox> ChatScrollBox;
	TSharedPtr<SGridPanel> MinesGridPanel;

	TArray<TSharedRef<SMineButton>> MinesweeperButtons;
	TArray<TSharedRef<SMineButton>> MinesweeperBombButtons;
	TArray<int32> MinesweeperField;
	int32 MinesweeperMines;
	int32 MinesweeperMaxRow;

	FText PromptToSend;
	
	UPythonBridge* PythonBridge;

	//SPEAKERS TEXTS OF CHAT BOX
	const FString& UserSpeakerText = "USER";
	const FString& AISpeakerText = "AI";
	const FString& JsonSpeakerText = "JSON";
	const FString& ErrorSpeakerText = "ERROR";

};
