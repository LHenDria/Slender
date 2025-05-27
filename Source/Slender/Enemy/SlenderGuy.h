// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlenderGuy.generated.h"

UCLASS()
class SLENDER_API ASlenderGuy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlenderGuy();
	void Teleport();
	FVector PLocation;
	UPROPERTY(EditAnywhere)
	float TP_Rate = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
