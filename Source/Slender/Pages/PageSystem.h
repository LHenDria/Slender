#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PageSystem.generated.h"

UCLASS()
class SLENDER_API APageSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	APageSystem();
	virtual void Tick(float DeltaTime) override;
	void IncrementPages();
	int GetPages();

protected:
	virtual void BeginPlay() override;
	int AllPages;
	int CollectedPages;
};
