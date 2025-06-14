#include "SlenderGuy.h"

#include "Components/BoxComponent.h"

ASlenderGuy::ASlenderGuy() {
	PrimaryActorTick.bCanEverTick = true;
	
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("SlenderHitbox"));
	RootComponent = Hitbox;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlenderMesh"));
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetupAttachment(RootComponent);
}

void ASlenderGuy::BeginPlay() {
	Super::BeginPlay();

}

void ASlenderGuy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

