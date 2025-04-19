#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
};
