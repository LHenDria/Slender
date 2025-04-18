#include "Slender/Player/PlayerEntity.h"
#include "Camera/CameraComponent.h"

APlayerEntity::APlayerEntity() {
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerEntityCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
}

void APlayerEntity::BeginPlay() {
	Super::BeginPlay();
	
}

void APlayerEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APlayerEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerEntity::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerEntity::MoveY);
	PlayerInputComponent->BindAxis("CameraX", this, &APlayerEntity::TurnX);
	PlayerInputComponent->BindAxis("CameraY", this, &APlayerEntity::TurnY);
}

void APlayerEntity::MoveX(float Input) {
	FVector xDirection = GetActorForwardVector();
	AddMovementInput(xDirection, Input);
}

void APlayerEntity::MoveY(float Input) {
	FVector yDirection = GetActorRightVector();
	AddMovementInput(yDirection, Input);
}

void APlayerEntity::TurnX(float Input) {
	AddControllerYawInput(Input);
}

void APlayerEntity::TurnY(float Input) {
	AddControllerPitchInput(Input);
}

