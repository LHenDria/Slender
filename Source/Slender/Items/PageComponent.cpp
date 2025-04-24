#include "Slender/Items/PageComponent.h"

UPageComponent::UPageComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UPageComponent::BeginPlay() {
	Super::BeginPlay();
	
}

void UPageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPageComponent::PickupPage() {
	
}

void UPageComponent::RemovePage() {
	
}



