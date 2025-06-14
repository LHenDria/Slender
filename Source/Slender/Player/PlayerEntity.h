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
	UPROPERTY()
	class APageSystem* PageSystem;
	FTimerHandle HealthDamageTimer;
	FTimerHandle HealthRestoreTimer;

	UPROPERTY(EditAnywhere)
	int Health = 200;
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 500.0f;
	UPROPERTY(EditAnywhere)
	float SprintSpeed = 800.0f;
	
	void MoveX(float Input);
	void MoveY(float Input);
	void TurnX(float Input);
	void TurnY(float Input);
	void StartSprint();
	void StopSprint();
	void ToggleFlashlight();
	void DetectPage();
	void GameOverState();
	void LowerHealth();
	void RestoreHealth();
	bool CheckIfSlenderInSight();
	bool CheckIfSlenderIsClose();
};
