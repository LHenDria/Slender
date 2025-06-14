
#include "SlenderGuy.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slender/Player/PlayerEntity.h"


void ASlenderGuy::Teleport()
{
	float randomx = 400 + rand() % 1000;
	float randomy = 400 + rand() % 1000;
	float direction = rand() % (8 + 1) - 1;
	if (direction <=2)
	{
		SetActorLocation(PLocation + FVector(randomx,randomy,0.f));
		SetActorRotation(FacingRotator);
	}
	else if (2 < direction && direction <= 4)
	{
		SetActorLocation(PLocation + FVector(-randomx,-randomy,0.f));
		SetActorRotation(FacingRotator);
	}
	else if (4 < direction && direction <= 6)
	{
		SetActorLocation(PLocation + FVector(-randomx,randomy,0.f));
		SetActorRotation(FacingRotator);
	}
	else
	{
		SetActorLocation(PLocation + FVector(randomx,-randomy,0.f));
		SetActorRotation(FacingRotator);
	}
}


// Sets default values
ASlenderGuy::ASlenderGuy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlenderGuy::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PLocation = myCharacter->GetActorLocation();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASlenderGuy::Teleport, TP_Rate, true);
}

// Called every frame
void ASlenderGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PLocation = myCharacter->GetActorLocation();
	FacingRotator = UKismetMathLibrary::FindLookAtRotation(PLocation, GetActorLocation());
}

