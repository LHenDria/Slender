#include "Slender/Menus/WinnerState.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UWinnerState::NativeConstruct() {
	if (RetryButton) {
		RetryButton->OnClicked.AddDynamic(this, &UWinnerState::OnClicked);
	}
}

void UWinnerState::OnClicked() {
	UWorld* World = GetWorld();
	if (World) {
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}