#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaticScreen.generated.h"

UCLASS()
class SLENDER_API UStaticScreen : public UUserWidget {
	GENERATED_BODY()
	
	virtual void NativeConstruct();
};
