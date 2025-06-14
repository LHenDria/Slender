// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverState.generated.h"


UCLASS()
class SLENDER_API UGameOverState : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RetryButton;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClicked();
};
