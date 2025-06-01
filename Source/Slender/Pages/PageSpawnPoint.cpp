#include "Slender/Pages/PageSpawnPoint.h"

#include "Components/ArrowComponent.h"

APageSpawnPoint::APageSpawnPoint() {
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	RootComponent = Arrow;
}

void APageSpawnPoint::BeginPlay() {
	Super::BeginPlay();
	
}

void APageSpawnPoint::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

