#pragma once
#include "Entity.h"
#include "AnimationManager.h"
#include "SfxManager.h"

class Player;
class Texture;

class Enemy :
    public Entity
{
public:

    static Texture* g_basicEnemySpriteSheet;
    static Player* g_Target;
private:
    float m_speed;
    float m_size;
    
    bool m_jumps;
    float m_jumpRange;
    float m_jumpCooldown{2.0f};
    float m_jumpTimer{0.f};
    bool m_anticipates;

    AnimationManager m_animationManager; 
    SfxManager m_sfxManager;
public:
    Enemy(Vector2f position, bool jumps, bool anticipates, float speed ,float size , float health);
    void Update(float elapsedSec) override;
    void Draw() const override;
    int GetScore();

};

