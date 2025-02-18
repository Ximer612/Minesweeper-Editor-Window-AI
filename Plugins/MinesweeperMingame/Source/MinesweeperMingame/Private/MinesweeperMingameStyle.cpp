// Copyright 2025, Ximer - Marco Baldini, All rights reserved

#include "MinesweeperMingameStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperMingameStyle::StyleInstance = nullptr;

void FMinesweeperMingameStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMinesweeperMingameStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMinesweeperMingameStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MinesweeperMingameStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMinesweeperMingameStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MinesweeperMingameStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MinesweeperMingame")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MinesweeperMingame.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FMinesweeperMingameStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMinesweeperMingameStyle::Get()
{
	return *StyleInstance;
}
