#pragma once
#include "Projectile.h"
class Pizza :
    public Projectile
{
public:
    static Texture* g_pPizzaTexture;
    Pizza(Vector2f position, Vector2f velocity, float angle, Entity* parent = nullptr);
    Pizza(const Pizza& other) = default;


    virtual void Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles) override;
    virtual void Draw()const override;

    virtual Projectile* Clone() const override;
    virtual float GetProjectileSpeed() override;
};

