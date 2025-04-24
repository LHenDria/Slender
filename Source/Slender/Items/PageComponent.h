#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLENDER_API UPageComponent : public UActorComponent {
	GENERATED_BODY()
public:	
	UPageComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PickupPage();
	void RemovePage();
	
protected:
	virtual void BeginPlay() override;
};
