#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PageEntity.generated.h"

UCLASS()
class SLENDER_API APageEntity : public AActor {
	GENERATED_BODY()
	
public:	
	APageEntity();
	virtual void Tick(float DeltaTime) override;
	void PickUpPage();

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Hitbox;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	virtual void BeginPlay() override;
};
