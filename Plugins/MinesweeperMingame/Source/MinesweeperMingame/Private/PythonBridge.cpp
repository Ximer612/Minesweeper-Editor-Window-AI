// Copyright 2025, Ximer - Marco Baldini, All rights reserved


#include "PythonBridge.h"

UPythonBridge* UPythonBridge::Get()
{

    TArray<UClass*> PythonBridgeClasses;
    GetDerivedClasses(UPythonBridge::StaticClass(), PythonBridgeClasses);
    int32 NumClasses = PythonBridgeClasses.Num();
    if (NumClasses > 0)
    {
        return Cast<UPythonBridge>(PythonBridgeClasses[NumClasses - 1]->GetDefaultObject());
    }
    return nullptr;
}
void UPythonBridge::UpdateTextBlock(const FString& NewString) const
{
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([this, &NewString]() {

        if (ChatTextBlock)
        {
            ChatTextBlock->SetText(FText::FromString(ChatTextBlock->GetText().ToString() + NewString));
        }

        OnUpdatedText.ExecuteIfBound();

		}, TStatId(), nullptr, ENamedThreads::GameThread);

	Task->Wait();
}
