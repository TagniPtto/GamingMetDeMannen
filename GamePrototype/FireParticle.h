#pragma once
#include "Projectile.h"
class FireParticle :
    public Projectile
{
public:
    int m_formcount{3};
    int m_form{};
public:
    FireParticle(Vector2f position = {}, Vector2f velocity = {}, float angle = 0, Entity* parent = nullptr);
    FireParticle(const FireParticle& other) = default;

    virtual void Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles) override;
    virtual void Draw()const override;
    virtual Projectile* Clone() const override;
};

