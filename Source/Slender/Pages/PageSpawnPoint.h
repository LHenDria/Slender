#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PageSpawnPoint.generated.h"

UCLASS()
class SLENDER_API APageSpawnPoint : public AActor {
	GENERATED_BODY()
	
public:	
	APageSpawnPoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* Arrow;
};
