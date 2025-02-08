// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperMingame.h"
#include "MinesweeperMingameStyle.h"
#include "MinesweeperMingameCommands.h"

#include "LevelEditor.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

#include "PythonBridge.h"
#include "PythonResult.h"

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
	//button to ask to ai
	// 
	//FText WidgetText = FText::Format(
	//	LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
	//	FText::FromString(TEXT("FMinesweeperMingameModule::OnSpawnPluginTab")),
	//	FText::FromString(TEXT("MinesweeperMingame.cpp"))
	//	);

	const FMargin ContentPadding = FMargin(500.f, 300.f);
	const FText WritePrompText = LOCTEXT("SendPromptAI", "Send prompt!");

	//return SNew(SMinesweeperDockTab);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				+SVerticalBox::Slot()
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FColor::Purple)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.FillWidth(0.9f)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Bottom)
							[
								SNew(SEditableTextBox)
									.OnTextChanged_Lambda([this](const FText& InText){
									OnChangedPromptText(InText);
									})//(FOnTextChanged::CreateSP(this, &OnChangedPromptText))
							]
							+ SHorizontalBox::Slot()
							.FillWidth(0.3f)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Bottom)
							[
								SNew(SButton)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.OnClicked_Lambda(
									[this]() -> FReply
									{
										UE_LOG(LogTemp, Warning, TEXT("Started!"));
										AsyncThread([this]() 
											{
												UE_LOG(LogTemp, Warning, TEXT("Inside new thread?!"));

												UPythonBridge* bridge = UPythonBridge::Get();
												FPythonResult response = bridge->AskToAIPython(PromptText.ToString());

												FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&response]() {
													UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *response.Response);
													UE_LOG(LogTemp, Warning, TEXT("Json response: %s"), *response.JsonResponse);
													}, TStatId(), nullptr, ENamedThreads::GameThread);

												Task->Wait();
											});


										return FReply::Handled();
									})
									[
										SNew(STextBlock)
										.Text(WritePrompText)
								]
							]
						]
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Top)
					[
						SNew(SButton)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Top)
						.Text(LOCTEXT("SendAgainLastPromptAI","Send again last prompt!"))
					]

				]
					//.SizeParam(FSizeParam(FSizeParam::SizeRule_Stretch,1.f,1.f))
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

void FMinesweeperMingameModule::OnChangedPromptText(const FText& InText)
{
	PromptText = InText;
}

FReply FMinesweeperMingameModule::SendPrompt()
{
//	//UE_LOG(LogTemp, Warning, TEXT("Started!"));
//	//AsyncThread([this]()
//	//	{
//	//		UE_LOG(LogTemp, Warning, TEXT("Inside new thread?!"));
//	//		UPythonBridge* bridge = UPythonBridge::Get();
//	//		FString response = bridge->FunctionImplementedInPython(PromptText.ToString());
//
//	//		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([response]() {
//	//			UE_LOG(LogTemp, Warning, TEXT("Response from another thread: %s"), *response);
//	//			}, TStatId(), nullptr, ENamedThreads::GameThread);
//
//	//		Task->Wait();
//	//	});
//
//	//UE_LOG(LogTemp, Warning, TEXT("Passed!"));
//
	return FReply::Handled();
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