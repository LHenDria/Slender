#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Slender/Enemy/SlenderGuy.h"
#include "PlayerEntity.generated.h"

UCLASS()
class SLENDER_API APlayerEntity : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerEntity();
	
protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
	class USpotLightComponent* Flashlight;
	UPROPERTY(EditAnywhere)
	class ASlenderGuy* guy;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	class TSubclassOf<UUserWidget> UserWidget;
	UPROPERTY()
	class UUserWidget* GameOverScreen;

	UPROPERTY(EditAnywhere, Category = "UI")
	class TSubclassOf<UUserWidget> WinnerWidget;
	UPROPERTY()
	class UUserWidget* WinnerScreen;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	class TSubclassOf<UUserWidget> StaticWidget;
	UPROPERTY()
	class UUserWidget* Static;
	
	UPROPERTY()
	class APageSystem* PageSystem;
	FTimerHandle FlashlightTimer;
	FTimerHandle HealthDamageTimer;
	FTimerHandle HealthRestoreTimer;
	FTimerHandle JumpscareTimer;
	FTimerHandle WinnerTimer;
	bool trigger = false;

	bool isOutOfBattery = false;
	bool should_be_flipped = true;
	bool should_static_play = false;
	
	UPROPERTY(EditAnywhere)
	int Health = 300;
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 380.0f;
	UPROPERTY(EditAnywhere)
	float SprintSpeed = 450.0f;
	UPROPERTY(EditAnywhere)
	float FlashlightBattery = 600.0f;
	
	UPROPERTY(EditAnywhere)
	USoundBase* FlashClick;
	UPROPERTY(EditAnywhere)
	USoundBase* StaticNoise;
	UPROPERTY(EditAnywhere)
	USoundBase* PagePickup;
	
	UPROPERTY(EditAnywhere)
	USoundBase* Steps1;
	UPROPERTY(EditAnywhere)
	USoundBase* Steps2;
	UPROPERTY(EditAnywhere)
	USoundBase* Steps3;
	UPROPERTY()
	UAudioComponent* StepsAudioComponent;
	
	void PlayStepsSound();
	void MoveX(float Input);
	void MoveY(float Input);
	void TurnX(float Input);
	void TurnY(float Input);
	void ExitGame();
	void StartSprint();
	void StopSprint();
	void ToggleFlashlight();
	void DisableFlash();
	void DetectPage();
	void GameOverState();
	void WinnerState();
	void LowerHealth();
	void RestoreHealth();
	bool CheckIfSlenderInSight();
	bool CheckIfSlenderIsClose();
	void Jumpscare();
};
