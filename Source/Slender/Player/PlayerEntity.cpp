#include "Slender/Player/PlayerEntity.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slender/Enemy/SlenderGuyMovementSystem.h"
#include "Slender/Pages/PageEntity.h"
#include "Slender/Pages/PageSystem.h"


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
	this->guy = static_cast<ASlenderGuy*>(UGameplayStatics::GetActorOfClass(GetWorld(), ASlenderGuy::StaticClass()));
	this->PageSystem = static_cast<APageSystem*>(UGameplayStatics::GetActorOfClass(GetWorld(), APageSystem::StaticClass()));
	GetWorldTimerManager().SetTimer(FlashlightTimer, this, &APlayerEntity::DisableFlash, FlashlightBattery, false);
	GameOverScreen = CreateWidget(GetWorld(), UserWidget);
	Static = CreateWidget(GetWorld(), StaticWidget);
}

bool APlayerEntity::CheckIfSlenderInSight() {
	if (!Flashlight->IsVisible()) {
		return false;
	}
	FVector flashlight_origin = Flashlight->GetComponentLocation();
	FVector flashlight_direction = Flashlight->GetForwardVector();
	float max_distance = Flashlight->AttenuationRadius;
	float cone_angle = Flashlight->OuterConeAngle;
	
	FVector ToSlender = (guy->GetActorLocation() - flashlight_origin);
	float distance = ToSlender.Size();

	if (distance <= max_distance) {
		ToSlender.Normalize();
		float dot = FVector::DotProduct(flashlight_direction, ToSlender);
		float angle = FMath::RadiansToDegrees(FMath::Acos(dot));

		if (angle <= cone_angle) {
			return true;
		}
	}
	return false;
}

bool APlayerEntity::CheckIfSlenderIsClose() {
	FVector player_location = this->GetActorLocation();

	float distance = FVector::Dist(player_location, guy->GetActorLocation());
	if (distance <= 400.0f) {
		return true;
	}
	return false;
}

void APlayerEntity::Jumpscare()
{
	Static->SetColorAndOpacity(FLinearColor(1,1,1,2));
	Static->AddToViewport();
	this->GetCharacterMovement()->GravityScale = 0;
	this->SetActorLocation(this->GetActorLocation() + Flashlight->GetForwardVector() * 100 + FVector(0, 0, 83));
	UGameplayStatics::PlaySound2D(this, StaticNoise, 1);
	FTimerHandle jumpscareTimerHandle;
	GetWorldTimerManager().SetTimer(jumpscareTimerHandle, this, &APlayerEntity::GameOverState, 1.3f, false);
}


void APlayerEntity::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if (CheckIfSlenderInSight() || CheckIfSlenderIsClose()) {
		if (!GetWorldTimerManager().IsTimerActive(HealthDamageTimer)) {
			GetWorldTimerManager().ClearTimer(HealthRestoreTimer);
			GetWorldTimerManager().SetTimer(HealthDamageTimer, this, &APlayerEntity::LowerHealth, 0.5f,true);
		}
	} else {
		if (!GetWorldTimerManager().IsTimerActive(HealthRestoreTimer)) {
			GetWorldTimerManager().ClearTimer(HealthDamageTimer);
			GetWorldTimerManager().SetTimer(HealthRestoreTimer, this, &APlayerEntity::RestoreHealth, 1.0f,true);
		}
	}
	if (this->IsOverlappingActor(guy)) {
		this->Health = 0;
	}
	if (this->Health <= 0)
	{
		if (!GetWorldTimerManager().IsTimerActive(JumpscareTimer) && trigger == false)
		{
			GetWorldTimerManager().SetTimer(JumpscareTimer, this, &APlayerEntity::Jumpscare, 2.0f, false);
			guy->SetActorLocationAndRotation((this->GetActorLocation() + Camera->GetForwardVector() * 200), UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), guy->GetActorLocation()));
			DestroyPlayerInputComponent();
			Static->SetColorAndOpacity(FLinearColor(1,1,1,1));
			Static->AddToViewport();
			UGameplayStatics::PlaySound2D(this, StaticNoise, 0.6f);
			trigger = true;
		}
	}
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
	if (isOutOfBattery) {
		return;
	}
	UGameplayStatics::PlaySound2D(this, FlashClick, 0.7f);
	Flashlight->ToggleVisibility();
	GetWorldTimerManager().PauseTimer(FlashlightTimer);
	if (Flashlight->IsVisible()) {
		GetWorldTimerManager().UnPauseTimer(FlashlightTimer);
	}
}

void APlayerEntity::DisableFlash()
{
	Flashlight->SetVisibility(false);
	this->isOutOfBattery = true;
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

void APlayerEntity::GameOverState() {
	UE_LOG(LogTemp, Display, TEXT("Game Over"));
	Camera->DestroyComponent();
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	GameOverScreen->AddToViewport();
}

void APlayerEntity::LowerHealth() {
	Health = Health - SlenderHealthLossPerSecond[PageSystem->GetPages()] / 2;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Slendy in sight, uh oh."));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Took damage: %d"), SlenderHealthLossPerSecond[PageSystem->GetPages()] / 2));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Heatlh: %d"), this->Health));
	Static->SetColorAndOpacity(FLinearColor(1,1,1,0.5));
	Static->AddToViewport();
	UGameplayStatics::PlaySound2D(this, StaticNoise, 0.4f);
	
}

void APlayerEntity::RestoreHealth() {
	if (this->Health < 200) {
		this->Health = this->Health + 20;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Przysyla mnie Lewus, przynioslem ci wode.!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Heatlh: %d"), this->Health));
		Static->RemoveFromViewport();
		if (this->Health > 200) {
			this->Health = 200;
		}
	}
}
