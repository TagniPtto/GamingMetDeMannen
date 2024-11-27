#pragma once
#include <list>

class Texture;
class SoundStream;


class TextureManager;
class AnimationManager;

class Player;
class Projectile;
class Enemy;

class Level
{
public:

	int m_enemyCounter{0};
	int m_waveCounter{1};
	int m_score{ 0 };


	float m_waveTimer{ 0 };
	float m_cooldownTimer{ 0 };


	float m_coolDownDuration{3.0f};

	bool coolDown{ false };



	static float mouseX, mouseY;
	static bool leftMouse, rightMouse;
	Rectf m_viewport;

	std::list<Projectile*> g_projectiles;
	std::list<Enemy*> g_enemies;


	Player* g_pPlayer{ nullptr };
	Texture* g_pCrosshair{ nullptr };
	Texture* g_pBackground{ nullptr };
	TextureManager* g_pTextureManager{nullptr};


	SoundStream* g_pMusic{ nullptr };

public:
	Level(Rectf viewport,Player* player, TextureManager* txtManager);
	~Level();


	void SpawnWave(float elapsedSec);
	void Reset();
	void Update(float elapsedSec);
	void Draw() const;



	std::list<Projectile*>& GetProjectiles();
	std::list<Enemy*>& GetEnemies();
private:
	void DrawUI() const;
	void DrawEntities() const;
	void DrawMenu()const;
	void DrawNumber(int number,  int maxNumberDisplaySize , const Rectf& dst) const;
	void UpdateEntities(float elapsedSec);
};

