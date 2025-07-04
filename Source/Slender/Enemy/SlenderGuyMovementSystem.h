#pragma once

#include "CoreMinimal.h"
#include "SlenderGuyMovementNode.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "Slender/Player/PlayerEntity.h"
#include "SlenderGuyMovementSystem.generated.h"

static TArray<int> SlenderHealthLossPerSecond = {0, 70, 80, 90, 100, 110, 130, 180, 999999};
static TArray<float> SlenderTeleportInSeconds = {0.0f, 10.0f, 9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f};
static TArray<int> SlenderTeleportChance = {0, 1, 2, 3, 4, 5, 7, 9, 10};

UCLASS()
class SLENDER_API ASlenderGuyMovementSystem : public AActor {
	GENERATED_BODY()
	
public:	
	ASlenderGuyMovementSystem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void SlenderWeightDefault();
	void SlenderWeightMovementOppurtinity();
	void SlenderWeightFlashlight();
	void SlenderWeightHidden();
	void SlenderWeightCollision();
	void SlenderWeightSafeZone();

	void TeleportSlender();
	
	FTimerHandle SlenderTeleportTimer;
	UPROPERTY(EditAnywhere)
	TArray<ASlenderGuyMovementNode*> SpawnedNodesArray;
	UPROPERTY(EditAnywhere)
	ASlenderGuy *slender;
	UPROPERTY(EditAnywhere)
	APlayerEntity *player;
	UPROPERTY(EditAnywhere)
	USpotLightComponent *player_flashlight;
	UPROPERTY(EditAnywhere)
	APageSystem *page_system;
	UPROPERTY(EditAnywhere)
	int NumberOfNodesOnSide = 18;
	UPROPERTY(EditAnywhere)
	float SpacingBetweenNodes = 120.0f;
};
