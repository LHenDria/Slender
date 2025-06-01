#include "PageSpawnSystem.h"

#include "PageEntity.h"
#include "Kismet/GameplayStatics.h"

APageSpawnSystem::APageSpawnSystem() {

	PrimaryActorTick.bCanEverTick = true;

}

void APageSpawnSystem::SpawnPage(APageSpawnPoint *spawn_point) {
	FVector spawn_location = spawn_point->GetActorLocation();
	FRotator spawn_rotation = spawn_point->GetActorRotation();
	FActorSpawnParameters spawn_params;
	spawn_params.Owner = this;

	TSubclassOf<APageEntity> spawned_page_bp_class = LoadClass<APageEntity>(nullptr, TEXT("/Game/Pages/BP_PageEntity.BP_PageEntity_C"));
	APageEntity *spawned_page = GetWorld()->SpawnActor<APageEntity>(spawned_page_bp_class, spawn_location, spawn_rotation, spawn_params);
	if (spawned_page) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PageSpawnSystem: Spawned a page."));
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PageSpawnSystem: Spawnedn't a page."));
	}
}

void APageSpawnSystem::BeginPlay() {
	Super::BeginPlay();
	
	TArray<AActor *> pages;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APageSpawnPoint::StaticClass(), pages);

	for (int i = 0; i < NUMBER_OF_PAGES; i++) {
		int random_number = FMath::RandRange(0, pages.Num() - 1);
		APageSpawnPoint *spawn_point = Cast<APageSpawnPoint>(pages[random_number]);
		SpawnPage(spawn_point);
		pages.RemoveAt(random_number);
	}
}

void APageSpawnSystem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

