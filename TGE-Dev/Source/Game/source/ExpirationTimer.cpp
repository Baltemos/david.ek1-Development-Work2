#include "ExpirationTimer.h"
#include "Entity.h"

void ExpirationTimer::Read(const nlohmann::json& someData)
{
	myTimer = someData["Timer"];
}

void ExpirationTimer::Update(float aDeltaTime)
{
	myTimer -= aDeltaTime;
	if (myTimer < 0)
	{
		GetEntity()->Destroy();
	}
}
