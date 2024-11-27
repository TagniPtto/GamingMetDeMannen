#pragma once
#include "Projectile.h"
class PizzaSlice :
    public Projectile
{
public:
    static Texture* g_pPizzaSliceTexture;
    PizzaSlice(Vector2f position, Vector2f velocity, float angle, Entity* parent = nullptr);
    PizzaSlice(const PizzaSlice& other) = default;
    
    virtual void Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles) override;
    virtual void Draw()const override;


    virtual Projectile* Clone() const override;
    virtual float GetProjectileSpeed() override;
};

