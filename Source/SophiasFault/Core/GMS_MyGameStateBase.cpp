#include "GMS_MyGameStateBase.h"

AGMS_MyGameStateBase::AGMS_MyGameStateBase()
{
	_onChase = false;
}

void AGMS_MyGameStateBase::BeginPlay() 
{
	Super::BeginPlay();

	_onChase = true;
}