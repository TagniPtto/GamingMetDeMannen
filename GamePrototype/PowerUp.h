#pragma once

#include "Entity.h"
class PowerUp:public Entity
{
public:
	virtual void Update(float elapsedSec)override;
	virtual void Draw() const override;
};

