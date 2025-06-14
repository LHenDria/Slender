#include "Slender/Enemy/SlenderGuyMovementSystem.h"

#include "SlenderGuyMovementNode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Slender/Pages/PageSystem.h"

ASlenderGuyMovementSystem::ASlenderGuyMovementSystem() {
 	PrimaryActorTick.bCanEverTick = true;
	
}

void ASlenderGuyMovementSystem::BeginPlay() {
	Super::BeginPlay();

	this->player = Cast<APlayerEntity>(GetWorld()->GetFirstPlayerController()->GetPawn());
	this->player_flashlight = Cast<USpotLightComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<USpotLightComponent>());
	TArray<AActor *> slenders;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASlenderGuy::StaticClass(), slenders);
	this->slender = Cast<ASlenderGuy>(slenders[0]);
	this->page_system = static_cast<APageSystem*>(UGameplayStatics::GetActorOfClass(GetWorld(), APageSystem::StaticClass()));
	
	FVector player_location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector spawn_location = player_location;
	FRotator spawn_rotation = FRotator(0, 0, 0);
	FActorSpawnParameters spawn_params;
	spawn_params.Owner = this;

	float half_grid_size = (NumberOfNodesOnSide - 1) * SpacingBetweenNodes * 0.5f;

	TSubclassOf<ASlenderGuyMovementNode> spawned_node_bp_class = LoadClass<ASlenderGuyMovementNode>(nullptr, TEXT("/Game/Enemy/BP_SlenderGuyMovementNode.BP_SlenderGuyMovementNode_C"));
	for (int i = 0; i < NumberOfNodesOnSide; i++) {
		for (int j = 0; j< NumberOfNodesOnSide; j++) {
			FVector tmp_spawn_location = spawn_location + FVector(i * SpacingBetweenNodes - half_grid_size, j * SpacingBetweenNodes - half_grid_size, 0);
			ASlenderGuyMovementNode* spawned_node = GetWorld()->SpawnActor<ASlenderGuyMovementNode>(spawned_node_bp_class, tmp_spawn_location, spawn_rotation, spawn_params);

			spawned_node->SetDisplacementLocation(FVector(i * SpacingBetweenNodes - half_grid_size, j * SpacingBetweenNodes - half_grid_size, 0));
			spawned_node->SetWeight(1);
			
			if (spawned_node) {
				SpawnedNodesArray.Add(spawned_node);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SlenderGuyMovementSystem: Spawned a node."));
			} else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SlenderGuyMovementSystem: Spawnedn't a node."));
			}
		}
	}
}

void ASlenderGuyMovementSystem::SlenderWeightDefault() {
	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		node->SetWeight(1);
	}
}

void ASlenderGuyMovementSystem::SlenderWeightFlashlight() {
	if (!player_flashlight->IsVisible()) {
		return;
	}
	
	FVector flashlight_origin = player_flashlight->GetComponentLocation();
	FVector flashlight_direction = player_flashlight->GetForwardVector();
	float max_distance = player_flashlight->AttenuationRadius;
	float cone_angle = player_flashlight->OuterConeAngle;
	
	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		FVector ToNode = (node->GetActorLocation() - flashlight_origin);
		float distance = ToNode.Size();

		if (distance <= max_distance) {
			ToNode.Normalize();
			float dot = FVector::DotProduct(flashlight_direction, ToNode);
			float angle = FMath::RadiansToDegrees(FMath::Acos(dot));

			if (angle <= cone_angle) {
				node->SetWeight(0);
			}
		}
	}
}

void ASlenderGuyMovementSystem::SlenderWeightMovementOppurtinity() {
	FVector velocity = player->GetVelocity();
	if (velocity.IsNearlyZero()) {
		return;
	}
	FVector player_direction = velocity.GetSafeNormal();
	float max_dot = 0.9f;

	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		FVector ToNode = node->GetActorLocation() - player->GetActorLocation();
		ToNode.Normalize();

		float dot = FVector::DotProduct(player_direction, ToNode);
		if (dot >= max_dot) {
			node->SetWeight(8);
		}
	}
}

void ASlenderGuyMovementSystem::SlenderWeightCollision() {
	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		FVector location = node->GetActorLocation();
		FHitResult hit;
		
		float radius = 10.0f;

		bool collision = UKismetSystemLibrary::SphereTraceSingle(this, location, location, radius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::None, hit, true);

		if (collision) {
			node->SetWeight(0);
		}
		
	}
}

void ASlenderGuyMovementSystem::SlenderWeightHidden() {
	FVector player_location = player->GetActorLocation();
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);

	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		FVector node_location = node->GetActorLocation();
		FHitResult hit;
		bool collision = GetWorld()->LineTraceSingleByChannel(hit, player_location, node_location, ECC_Visibility, params);
		if (collision) {
			node->SetWeight(3);
		}
	}
}

void ASlenderGuyMovementSystem::SlenderWeightSafeZone() {
	FVector player_location = player->GetActorLocation();
	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		float distance = FVector::Dist(player_location, node->GetActorLocation());
		if (distance <= 500.0f) {
			node->SetWeight(0);
		}
	}
}


void ASlenderGuyMovementSystem::TeleportSlender() {
	FVector player_location = player->GetActorLocation();
	float distance = FVector::Dist(player_location, slender->GetActorLocation());
	
	if (page_system->GetPages() == 8 || page_system->GetPages() == 0) {
		FVector under_map(0.0f, 0.0f, -99999.0f);
		slender->SetActorLocation(under_map);
		return;
	}
	
	if (distance <= 1500.0f) {
		int random_tp_chance = FMath::FRandRange(0, 10);
		if (random_tp_chance >= SlenderTeleportChance[page_system->GetPages()]) {
			return;
		}
	}
	
	TArray<ASlenderGuyMovementNode*> valid_nodes;
	TArray<int> weights;
	int total_weight = 0.0f;
	
	for (ASlenderGuyMovementNode* node : SpawnedNodesArray) {
		int weight = node->GetWeight();
		if (weight > 0) {
			valid_nodes.Add(node);
			weights.Add(weight);
		}
	}

	for (int weight : weights) {
		total_weight += weight;
	}

	float random_node = FMath::FRandRange(0.0f, total_weight);
	float choose_weights = 0.0f;
	for (int i = 0; i < valid_nodes.Num(); ++i) {
		choose_weights += weights[i];
		if (random_node <= choose_weights) {
			slender->SetActorLocation(valid_nodes[i]->GetActorLocation());
			return;
		}
	}
}


void ASlenderGuyMovementSystem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	SlenderWeightDefault();
	SlenderWeightMovementOppurtinity();
	SlenderWeightFlashlight();
	SlenderWeightHidden();
	SlenderWeightCollision();
	SlenderWeightSafeZone();
	if (!GetWorldTimerManager().IsTimerActive(SlenderTeleportTimer)) {
		GetWorldTimerManager().SetTimer(SlenderTeleportTimer, this, &ASlenderGuyMovementSystem::TeleportSlender, SlenderTeleportInSeconds[page_system->GetPages()],true);
	}
}

