#include "Slender/Player/PlayerEntity.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Slender/Pages/PageEntity.h"

APlayerEntity::APlayerEntity() {
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxAcceleration = 999999;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerEntityCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(Camera);
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerEntity::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerEntity::StopSprint);
	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &APlayerEntity::ToggleFlashlight);
	PlayerInputComponent->BindAction("DetectPage", IE_Pressed, this, &APlayerEntity::DetectPage);
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

void APlayerEntity::StartSprint() {
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerEntity::StopSprint() {
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void APlayerEntity::ToggleFlashlight() {
	Flashlight->ToggleVisibility();
}

void APlayerEntity::DetectPage() {
	FVector camera_location = Camera->GetComponentLocation();
	FVector looking_direction = Camera->GetForwardVector();
	FVector end = camera_location + looking_direction * 200.0f;

	FHitResult hit_result;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hit_result, camera_location, end, ECC_Visibility, params)) {
		DrawDebugLine(GetWorld(), camera_location, end, FColor::Yellow, false, 3.0f, 0, 2.0f);
		APageEntity *page = Cast<APageEntity>(hit_result.GetActor());
		if (page) {
			page->PickUpPage();
		}
	}
}
