#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlenderGuy.generated.h"

UCLASS()
class SLENDER_API ASlenderGuy : public AActor {
	GENERATED_BODY()
	
public:	
	ASlenderGuy();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Hitbox;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	virtual void BeginPlay() override;
};
