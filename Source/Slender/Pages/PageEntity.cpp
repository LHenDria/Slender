#include "Slender/Pages/PageEntity.h"

#include "PageSystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

APageEntity::APageEntity() {
	PrimaryActorTick.bCanEverTick = true;
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	RootComponent = Hitbox;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetupAttachment(RootComponent);
}

void APageEntity::BeginPlay() {
	Super::BeginPlay();
}

void APageEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
}

void APageEntity::PickUpPage() {
	this->Destroy();
	APageSystem *page_system = Cast<APageSystem>(UGameplayStatics::GetActorOfClass(GetWorld() ,APageSystem::StaticClass()));
	if (page_system) {
		page_system->IncrementPages();
	}
}
