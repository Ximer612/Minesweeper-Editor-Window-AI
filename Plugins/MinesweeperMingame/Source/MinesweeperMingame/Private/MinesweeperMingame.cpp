// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperMingame.h"
#include "MinesweeperMingameStyle.h"
#include "MinesweeperMingameCommands.h"

#include "LevelEditor.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

#include <PythonBridge.h>

static const FName MinesweeperMingameTabName("MinesweeperMingame");

#define LOCTEXT_NAMESPACE "FMinesweeperMingameModule"

void FMinesweeperMingameModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperMingameStyle::Initialize();
	FMinesweeperMingameStyle::ReloadTextures();

	FMinesweeperMingameCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperMingameCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperMingameModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperMingameModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperMingameTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperMingameModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMinesweeperMingameTabTitle", "MinesweeperMingame"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperMingameModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperMingameStyle::Shutdown();

	FMinesweeperMingameCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperMingameTabName);
}

TSharedRef<SDockTab> FMinesweeperMingameModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	UE_LOG(LogTemp, Warning, TEXT("Started!"));
	AsyncThread([]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Inside new thread?!"));
			UPythonBridge* bridge = UPythonBridge::Get();
			FString response = bridge->FunctionImplementedInPython();

			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([response]() {
				UE_LOG(LogTemp, Warning, TEXT("Response from another thread: %s"), *response);
				}, TStatId(), nullptr, ENamedThreads::GameThread);

			Task->Wait();
		});

	UE_LOG(LogTemp, Warning, TEXT("Passed!"));


	//button to ask to ai
	// 
	//FText WidgetText = FText::Format(
	//	LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
	//	FText::FromString(TEXT("FMinesweeperMingameModule::OnSpawnPluginTab")),
	//	FText::FromString(TEXT("MinesweeperMingame.cpp"))
	//	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(FMargin(5.0f, 0.0f))
				.AutoWidth()
				[
					SNew(SButton)
						.Text(LOCTEXT("PaperExtractSpritesCancelButton", "Cancel"))
						.TextStyle(FAppStyle::Get(), "DialogButtonText")
						.HAlign(HAlign_Center)
						//.OnClicked(this, &FMinesweeperMingameModule::PluginButtonClicked)
				]

			]
		];

	//wait for response in another thread

//get response

//check if response is in a correct format

//generate field
}

void FMinesweeperMingameModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperMingameTabName);
}

void FMinesweeperMingameModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperMingameCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperMingameCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperMingameModule, MinesweeperMingame)