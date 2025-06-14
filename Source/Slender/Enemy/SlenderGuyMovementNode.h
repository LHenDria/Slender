#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlenderGuyMovementNode.generated.h"

UCLASS()
class SLENDER_API ASlenderGuyMovementNode : public AActor {
	GENERATED_BODY()
	
public:	
	ASlenderGuyMovementNode();
	virtual void Tick(float DeltaTime) override;
	void SetWeight(int weight);
	int GetWeight();
	void SetDisplacementLocation(FVector location);

protected:
	virtual void BeginPlay() override;
	int Weight;
	FVector NodeDisplacement;
};
