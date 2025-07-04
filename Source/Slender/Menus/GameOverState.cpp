#include "Slender/Menus/GameOverState.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverState::NativeConstruct() {
	if (RetryButton) {
		RetryButton->OnClicked.AddDynamic(this, &UGameOverState::OnClicked);
	}
}

void UGameOverState::OnClicked() {
	UWorld* World = GetWorld();
	if (World) {
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}
