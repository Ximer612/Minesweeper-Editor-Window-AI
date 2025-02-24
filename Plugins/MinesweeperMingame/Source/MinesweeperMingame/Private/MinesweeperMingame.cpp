// Copyright 2025, Ximer - Marco Baldini, All rights reserved

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
#include "Json.h"

#include "SMineButton.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


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
	//UI to ask to ai

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
					.ColorAndOpacity(FColor::Yellow)
					.Padding(20.f)
					[
						SAssignNew(MinesGridPanel,SGridPanel)
					]
				]
				+SVerticalBox::Slot()
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.Padding(20.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Top)
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.HAlign(HAlign_Left)
							.VAlign(VAlign_Fill)
							[
								SNew(SButton)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.OnClicked_Raw(this, &FMinesweeperMingameModule::ClearMinesweeperMinigame)
								[
									SNew(STextBlock)
										.Text(LOCTEXT("MinesweeperClearGameButton", "Clear game!"))
								]
							]
							+SHorizontalBox::Slot()
							.AutoWidth()
							.HAlign(HAlign_Right)
							.VAlign(VAlign_Fill)
							[
								SNew(SButton)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.OnClicked_Raw(this, &FMinesweeperMingameModule::SendPrompt, true)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("SendAgainLastPromptAI", "Send again last prompt!"))
								]
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
								SAssignNew(SendPromptEditableTextBox, SMultiLineEditableTextBox)
								.Padding(5.f)
								.OnTextCommitted_Raw(this, &FMinesweeperMingameModule::SendLastPrompt)
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
			]
		];
}

void FMinesweeperMingameModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperMingameTabName);
}

FReply FMinesweeperMingameModule::SendPrompt(bool bResendLast)
{
	//getting the python implementation of this class
	if (!PythonBridge)
	{
		PythonBridge = UPythonBridge::Get();
		PythonBridge->OnUpdatedText.BindRaw(this, &FMinesweeperMingameModule::ScrollToEndChatBox);
	}

	//if the ai is thinking cannot sent another prompt
	if (PythonBridge->bIsAiThinking || (bResendLast && PromptToSend.IsEmpty()) || (!bResendLast && SendPromptEditableTextBox->GetText().IsEmpty()))
	{
		return FReply::Handled();
	}

	//if not pressend "send again last prompt"
	if (!bResendLast)
	{
		PromptToSend = SendPromptEditableTextBox->GetText();
		SendPromptEditableTextBox->SetText(FText::GetEmpty());
	}

	//add user text block
	AddTextBlockToScrollBox(PromptToSend.ToString(), FColor::Cyan, UserSpeakerText);

	//add ai stream text block
	PythonBridge->ChatTextBlock = AddTextBlockToScrollBox("", FColor::White, AISpeakerText).ToSharedPtr();

	//wait for ai response in another thread
	AsyncThread([this]()
		{
			PythonBridge->bIsAiThinking = true;

			//get response
			FPythonResult response = PythonBridge->AskToAIPython(PromptToSend.ToString());

			//returning into the game thread to access to UObjects
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&response,this]() {

				if (response.bError)
				{
					ChatScrollBox->RemoveSlot(PythonBridge->ChatTextBlock.ToSharedRef());
					AddTextBlockToScrollBox(response.Response, FColor::Red, ErrorSpeakerText);
					return;
				}

				//check if response is in a correct format
				if (response.JsonResponse != "none" && response.JsonResponse != "invalid")
				{
					ClearMinesweeperMinigame();

					AddTextBlockToScrollBox(response.JsonResponse, FColor::Green, JsonSpeakerText);

					//generate field
					TSharedPtr<FJsonObject> JsonParsed;
					TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(response.JsonResponse);
					if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
					{
						const TArray<TSharedPtr<FJsonValue>>& GridArray = JsonParsed->GetArrayField(TEXT("grid"));

						MinesweeperMaxRow = GridArray[0]->AsArray().Num();

						for (size_t y = 0; y < GridArray.Num(); y++)
						{
							const TArray<TSharedPtr<FJsonValue>>& JsonRow = GridArray[y]->AsArray();

							for (int32 x = 0; x < JsonRow.Num(); x++)
							{
								AddButtonMinesweeperMinigame(JsonRow[x].Get()->AsString(), x, y);
							}
						}

						//TODO: check if the grid has correct values
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Unable to parse Json!"));
					}
				}

				}, TStatId(), nullptr, ENamedThreads::GameThread);

			Task->Wait();

			PythonBridge->bIsAiThinking = false;

		});

	return FReply::Handled();
}

void FMinesweeperMingameModule::SendLastPrompt(const FText& InText, ETextCommit::Type CommitType)
{
	//if not pressed enter return
	if(CommitType != ETextCommit::Type::OnEnter)
	{
		return;
	}

	SendPrompt();
}

FReply FMinesweeperMingameModule::ClearMinesweeperMinigame()
{
	for (size_t i = 0; i < MinesweeperButtons.Num(); i++)
	{
		TSharedRef<SButton> Button = MinesweeperButtons[i];
		Button->SetVisibility(EVisibility::Collapsed);
	}

	MinesweeperBombButtons.Empty();
	MinesweeperButtons.Empty();
	MinesGridPanel->ClearChildren();
	MinesweeperField.Empty();
	MinesweeperMines = 0;

	return FReply::Handled();
}

void FMinesweeperMingameModule::AddButtonMinesweeperMinigame(const FString& InString, const int32& InColumn, const int32& InRow)
{
	TSharedRef<SMineButton> Button = SNew(SMineButton);

	int32 CellValue = 0;

	if (FCString::IsNumeric(*InString))
	{
		CellValue = FCString::Atoi(*InString);
	}
	else 
	{
		// if not a number then is a bomb!
		CellValue = INT32_MAX;
		MinesweeperMines++;
		Button->OnGameOver.BindRaw(this, &FMinesweeperMingameModule::MinesweeperGameOver);
		MinesweeperBombButtons.Add(Button);
	}

	MinesweeperField.Add(CellValue);

	int32 MyArrayIndex = MinesweeperButtons.Add(Button);

	auto NewScrollBoxSlot = MinesGridPanel->AddSlot(InColumn, InRow);
	NewScrollBoxSlot.AttachWidget(Button);
	NewScrollBoxSlot.HAlign(HAlign_Fill);
	NewScrollBoxSlot.VAlign(VAlign_Fill);

	Button->Value = CellValue;
	Button->Column = InColumn;
	Button->Row = InRow;	

	//add up and left bomb to the neighours
	//if not at left border
	if (MyArrayIndex % MinesweeperMaxRow != 0 && MinesweeperButtons.IsValidIndex(MyArrayIndex - 1) )
	{
		const TSharedRef<SMineButton> OtherMine = MinesweeperButtons[MyArrayIndex - 1];
		Button->AddNeighbour(OtherMine);
	}

	if (MinesweeperButtons.IsValidIndex(MyArrayIndex - MinesweeperMaxRow))
	{
		const TSharedRef<SMineButton> OtherMine = MinesweeperButtons[MyArrayIndex - MinesweeperMaxRow];
		Button->AddNeighbour(OtherMine);
	}
}

void FMinesweeperMingameModule::MinesweeperGameOver()
{
	AddTextBlockToScrollBox(LOCTEXT("GameOverMinesweeper","You lose! Ask me again for another match :)").ToString(), FColor::White, AISpeakerText);

	for (size_t i = 0; i < MinesweeperBombButtons.Num(); i++)
	{
		MinesweeperBombButtons[i].Get().MyText = FText::FromString("X");
	}

	for (size_t i = 0; i < MinesweeperButtons.Num(); i++)
	{
		MinesweeperButtons[i].Get().SetEnabled(false);
	}
}

void FMinesweeperMingameModule::ScrollToEndChatBox()
{
	ChatScrollBox->ScrollToEnd();
}

TSharedRef<STextBlock> FMinesweeperMingameModule::AddTextBlockToScrollBox(const FString& InText, const FSlateColor& InColor, const FString& SpeakerName)
{
	TSharedRef<STextBlock> NewChatTextBlock = SNew(STextBlock)
										.Text(FText::FromString("<" + SpeakerName + ">: " + InText))
										.ColorAndOpacity(InColor)
										.AutoWrapText(true);

	auto NewScrollBoxSlot = ChatScrollBox->AddSlot();
	NewScrollBoxSlot.AttachWidget(NewChatTextBlock);
	NewScrollBoxSlot.HAlign(HAlign_Fill);
	NewScrollBoxSlot.VAlign(VAlign_Fill);
	NewScrollBoxSlot.AutoSize();

	ScrollToEndChatBox();

	return NewChatTextBlock;
}

void FMinesweeperMingameModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

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