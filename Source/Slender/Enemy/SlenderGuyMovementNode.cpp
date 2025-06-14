#include "Slender/Enemy/SlenderGuyMovementNode.h"


ASlenderGuyMovementNode::ASlenderGuyMovementNode() {
 	PrimaryActorTick.bCanEverTick = true;

}

void ASlenderGuyMovementNode::BeginPlay() {
	Super::BeginPlay();
	
}

void ASlenderGuyMovementNode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	this->SetActorLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + NodeDisplacement);
	
	DrawDebugString(GetWorld(), GetActorLocation() + GetActorUpVector() * 55.0f, FString::Printf(TEXT("%d"), Weight), nullptr, FColor::White, 0.0f, false);
	if (Weight > 0.0f) {
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * 50.0f , FColor::Green, false, 0.0f, 0, 2.0f);
	} else {
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * 50.0f , FColor::Red, false, 0.0f, 0, 2.0f);
	}
}

void ASlenderGuyMovementNode::SetWeight(int weight) {
	this->Weight = weight;
}

int ASlenderGuyMovementNode::GetWeight() {
	return this->Weight;
}

void ASlenderGuyMovementNode::SetDisplacementLocation(FVector location) {
	this->NodeDisplacement = location;
}

