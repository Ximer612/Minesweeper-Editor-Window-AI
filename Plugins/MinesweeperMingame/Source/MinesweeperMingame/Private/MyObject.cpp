// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"

void SMyObject::Construct(const FArguments& InArgs)
{
	SButton::Construct(InArgs);

	//*MyText = FText::FromString(DefaultStringValue);

	ChildSlot[
		SAssignNew(TextBlock,STextBlock)
			.Text(FText::FromString(DefaultStringValue))
			.ColorAndOpacity(FColor::Red)
	];
}

void SMyObject::Press()
{
	//*MyText = FText::FromString("Bom");

	UE_LOG(LogTemp, Error, TEXT("[%i;%i] value is %d"),Column,Row,Value);
}
