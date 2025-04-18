#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	void MoveX(float Input);
	void MoveY(float Input);
	void TurnX(float Input);
	void TurnY(float Input);
};
