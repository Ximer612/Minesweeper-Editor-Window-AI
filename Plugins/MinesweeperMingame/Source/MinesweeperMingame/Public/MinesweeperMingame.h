// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

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
	void AddTextBlockToScrollBox(const FText& InText, const FSlateColor& InColor);

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<SEditableTextBox> SendPromptEditableTextBox;
	TSharedPtr<SScrollBox> ChatScrollBox;

	FText PromptToSend;
	bool bIsAiThinking;
};
