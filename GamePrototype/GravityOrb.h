#pragma once
#include "Projectile.h"
#include "AnimationManager.h"

class GravityOrb :
    public Projectile
    
{
public:
    const float m_force{ };
    const float m_rotationForce{  };
    AnimationManager m_animManager;

    static Rectf g_viewPort;

    static Texture* g_orbTexture;
    static Texture* g_pDisplayedBackground;
    static Texture* g_pReferencBackground;


    Vector2f prevPosition{m_Position};
public:

    GravityOrb(Vector2f position, Vector2f velocity, float angle, Entity* parent = nullptr , float force = 12000.f,float rotationForce = 10000.f);
    GravityOrb(const GravityOrb& other) = default;
    ~GravityOrb();


    virtual void Update(float elapsedSec, std::list<Enemy*>& enemies, std::list<Projectile*>& projectiles) override;
    virtual void Draw()const override;

    virtual Projectile* Clone() const override;
    virtual float GetProjectileSpeed() override;


    void DrawHole();
    void FillHole();

    Vector2f BlackHoleFunction(Vector2f in);
    float h(float x);
};

