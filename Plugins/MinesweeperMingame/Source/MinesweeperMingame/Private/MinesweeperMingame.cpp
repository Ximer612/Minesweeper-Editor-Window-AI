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

	const FMargin ContentPadding = FMargin(500.f, 300.f);
	const FText WritePrompText = LOCTEXT("SendPromptAI", "Send prompt!");

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
				[
					SNew(SBorder)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.ForegroundColor(FColor::Yellow)
					.BorderBackgroundColor(FColor::Magenta)
					.ColorAndOpacity(FColor::Blue)
					[
						SNew(SGridPanel)

					]
				]
				+SVerticalBox::Slot()
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.Padding(10.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Top)
						.AutoHeight()
						[
							SNew(SButton)
								.HAlign(HAlign_Fill)
								//.HAlign(HAlign_Right)
								//.VAlign(VAlign_Top)
								.VAlign(VAlign_Fill)
								.OnClicked_Raw(this, &FMinesweeperMingameModule::SendPrompt, true)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("SendAgainLastPromptAI", "Send again last prompt!"))
								]
						]
						+SVerticalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.FillHeight(1.f)
						[
							SAssignNew(ChatScrollBox,SScrollBox)
							
						]
						+SVerticalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.FillHeight(0.3f)
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.FillWidth(0.9f)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SAssignNew(SendPromptEditableTextBox, SEditableTextBox)
								.OnTextCommitted_Raw(this, &FMinesweeperMingameModule::SendLastPrompt)
								.BackgroundColor(FColor::Yellow)
								.ForegroundColor(FColor::Yellow)
							]
							+ SHorizontalBox::Slot()
							.FillWidth(0.3f)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.AutoWidth()
							[
								SNew(SButton)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								.OnClicked_Raw(this, &FMinesweeperMingameModule::SendPrompt,false)
								[
									SNew(STextBlock)
									.Text(WritePrompText)
								]
							]
						]
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

FReply FMinesweeperMingameModule::SendPrompt(bool bResendLast)
{
	if (bIsAiThinking)
	{
		return FReply::Handled();
	}


	if (!bResendLast && !SendPromptEditableTextBox->GetText().IsEmpty())
	{
		PromptToSend = SendPromptEditableTextBox->GetText();
	}

	AddTextBlockToScrollBox(FText::FromString("<Me>: " + PromptToSend.ToString()), FColor::Blue);

	AsyncThread([this]()
		{
			bIsAiThinking = true;

			UPythonBridge* bridge = UPythonBridge::Get();
			FPythonResult response = bridge->AskToAIPython(PromptToSend.ToString());

			UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *response.Response);
			UE_LOG(LogTemp, Warning, TEXT("Json response: %s"), *response.JsonResponse);

			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&response,this]() {
				
				FText ResponseText = FText::FromString("<AI>: " + response.Response);
				AddTextBlockToScrollBox(ResponseText, FColor::Red);

				if (response.JsonResponse != "none" && response.JsonResponse != "invalid")
				{
					FText ResponseJson = FText::FromString("<AI>: " + response.JsonResponse);
					AddTextBlockToScrollBox(ResponseJson, FColor::Magenta);
				}

				}, TStatId(), nullptr, ENamedThreads::GameThread);

			Task->Wait();

			bIsAiThinking = false;

		});

	return FReply::Handled();
}

void FMinesweeperMingameModule::SendLastPrompt(const FText& InText, ETextCommit::Type CommitType)
{
	if(CommitType != ETextCommit::Type::OnEnter)
	{
		return;
	}

	SendPrompt();
}

void FMinesweeperMingameModule::AddTextBlockToScrollBox(const FText& InText, const FSlateColor& InColor)
{
	TSharedRef<STextBlock> NewChatText = SNew(STextBlock)
										.Text(InText)
										.ColorAndOpacity(InColor)
										.AutoWrapText(true);

	auto NewScrollBoxSlot = ChatScrollBox->AddSlot();
	NewScrollBoxSlot.AttachWidget(NewChatText);
	NewScrollBoxSlot.HAlign(HAlign_Fill);
	NewScrollBoxSlot.VAlign(VAlign_Fill);
	NewScrollBoxSlot.AutoSize();

	ChatScrollBox->ScrollToEnd();
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