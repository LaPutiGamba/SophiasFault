#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "KeyItem.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API UKeyItem : public UItem
{
	GENERATED_BODY()
	
protected:
	virtual void Use(class ASophia* character) override;
};