#pragma once

#include "CoreMinimal.h"
#include "PageSpawnPoint.h"
#include "GameFramework/Actor.h"
#include "PageSpawnSystem.generated.h"

#define NUMBER_OF_PAGES 8

UCLASS()
class SLENDER_API APageSpawnSystem : public AActor {
	GENERATED_BODY()
	
public:	
	APageSpawnSystem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void SpawnPage(APageSpawnPoint *spawn_point);
	
};
