#include "Slender/Pages/PageSystem.h"

#include "PageEntity.h"
#include "Kismet/GameplayStatics.h"
#include "PageSpawnSystem.h"

APageSystem::APageSystem() {
	PrimaryActorTick.bCanEverTick = true;
}

void APageSystem::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor *> pages;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APageEntity::StaticClass(), pages);
	AllPages = pages.Num();
	CollectedPages = 0;
}

void APageSystem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APageSystem::IncrementPages() {
	CollectedPages++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Pages: %d/%d"), CollectedPages, NUMBER_OF_PAGES));
}

int APageSystem::GetPages() {
	return this->CollectedPages;
}


