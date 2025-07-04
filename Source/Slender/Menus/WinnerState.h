#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinnerState.generated.h"

UCLASS()
class SLENDER_API UWinnerState : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RetryButton;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClicked();
};
