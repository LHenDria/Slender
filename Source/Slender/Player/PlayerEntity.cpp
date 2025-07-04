#include "Slender/Player/PlayerEntity.h"

#include "EngineUtils.h"
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
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
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
	WinnerScreen = CreateWidget(GetWorld(), WinnerWidget);
	Static = CreateWidget(GetWorld(), StaticWidget);

	StepsAudioComponent = NewObject<UAudioComponent>(this);
	StepsAudioComponent->RegisterComponent();
	StepsAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
	this->GetCharacterMovement()->GravityScale = 0;
	this->SetActorLocation(this->GetActorLocation() + Flashlight->GetForwardVector() * 100 + FVector(0, 0, 83));
	UGameplayStatics::PlaySound2D(this, StaticNoise, 1);
	FTimerHandle jumpscareTimerHandle;
	GetWorldTimerManager().SetTimer(jumpscareTimerHandle, this, &APlayerEntity::GameOverState, 1.3f, false);
	should_static_play = true;
	Static->SetColorAndOpacity(FLinearColor(1,1,1,0.5));
	Static->AddToViewport();
}


void APlayerEntity::Tick(float DeltaTime) {
	if (should_static_play) {
		if (should_be_flipped) {
			Static->SetRenderScale(FVector2D(-1.0f ,1.0f));
			should_be_flipped = false;
		} else {
			Static->SetRenderScale(FVector2D(1.0f ,-1.0f));
			should_be_flipped = true;
		}
	}
	Super::Tick(DeltaTime);
	if (!this->GetVelocity().IsNearlyZero()) {
		PlayStepsSound();
	}
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
	if (this->Health <= 0) {
		if (!GetWorldTimerManager().IsTimerActive(JumpscareTimer) && trigger == false) {
			GetWorldTimerManager().SetTimer(JumpscareTimer, this, &APlayerEntity::Jumpscare, 2.0f, false);
			guy->SetActorLocationAndRotation((this->GetActorLocation() + Camera->GetForwardVector() * 200), UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), guy->GetActorLocation()));
			DestroyPlayerInputComponent();
			Static->SetColorAndOpacity(FLinearColor(1,1,1,1));
			Static->AddToViewport();
			UGameplayStatics::PlaySound2D(this, StaticNoise, 0.6f);
			trigger = true;
		}
	}
	if (this->PageSystem->GetPages() >= 8) {
		guy->SetActorLocation(FVector(0.0, 0.0, -9999999.0));
		if (!GetWorldTimerManager().IsTimerActive(WinnerTimer)) {
			GetWorldTimerManager().SetTimer(WinnerTimer, this, &APlayerEntity::WinnerState, 10.0f, false);
		}
	}
}

void APlayerEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerEntity::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerEntity::MoveY);
	PlayerInputComponent->BindAxis("CameraX", this, &APlayerEntity::TurnX);
	PlayerInputComponent->BindAxis("CameraY", this, &APlayerEntity::TurnY);
	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &APlayerEntity::ExitGame);
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
	params.bTraceComplex = true;
	params.AddIgnoredActor(this);

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (!ActorItr->IsA(APageEntity::StaticClass())) {
			params.AddIgnoredActor(*ActorItr);
		}
	}
	
	if (GetWorld()->LineTraceSingleByChannel(hit_result, camera_location, end, ECC_Visibility, params)) {
		DrawDebugLine(GetWorld(), camera_location, end, FColor::Yellow, false, 3.0f, 0, 2.0f);
		APageEntity *page = Cast<APageEntity>(hit_result.GetActor());
		if (page) {
			page->PickUpPage();
			UGameplayStatics::PlaySound2D(this, PagePickup, 1.0f);
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

void APlayerEntity::WinnerState() {
	UE_LOG(LogTemp, Display, TEXT("Winner is you!"));
	Camera->DestroyComponent();
	
	DestroyPlayerInputComponent();
	
	
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	WinnerScreen->AddToViewport();
}

void APlayerEntity::LowerHealth() {
	Health = Health - SlenderHealthLossPerSecond[PageSystem->GetPages()] / 2;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Slendy in sight, uh oh."));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Took damage: %d"), SlenderHealthLossPerSecond[PageSystem->GetPages()] / 2));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Heatlh: %d"), this->Health));
	UGameplayStatics::PlaySound2D(this, StaticNoise, 0.4f);
	should_static_play = true;
	Static->SetColorAndOpacity(FLinearColor(1,1,1,0.5));
	Static->AddToViewport();
}

void APlayerEntity::RestoreHealth() {
	if (this->Health < 200) {
		this->Health = this->Health + 20;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Przysyla mnie Lewus, przynioslem ci wode.!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Heatlh: %d"), this->Health));
		should_static_play = false;
		Static->RemoveFromViewport();
		if (this->Health > 200) {
			this->Health = 200;
		}
	}
}

void APlayerEntity::PlayStepsSound() {
	if (StepsAudioComponent && !StepsAudioComponent->IsPlaying()) {
		int32 random = FMath::RandRange(1, 3);
		switch (random) {
			case 1:
				StepsAudioComponent->SetSound(Steps1);	
				break;
			case 2:
				StepsAudioComponent->SetSound(Steps2);
				break;
			case 3:
				StepsAudioComponent->SetSound(Steps3);
				break;
			default:
				break;
		}
		StepsAudioComponent->Play();
	}
}

void APlayerEntity::ExitGame() {
	// bye bye
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}


