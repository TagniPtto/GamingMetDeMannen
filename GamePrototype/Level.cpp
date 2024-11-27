#include "pch.h"
#include "Level.h"

#include "utils.h"
#include "Texture.h"
#include "SoundStream.h"


#include "Player.h"
#include "Projectile.h"
#include "Pizza.h"
#include "PizzaSlice.h"
#include "Enemy.h";
#include "TextureManager.h"

float Level::mouseX;
float Level::mouseY;
bool Level::leftMouse;
bool Level::rightMouse;

extern bool g_gameIsRunning;

Level::Level(Rectf viewport, Player* player, TextureManager* txtManager) :
	g_pTextureManager(txtManager),
	g_pMusic(new SoundStream("Music.mp3")),
	g_pPlayer(player),
	m_viewport(viewport)
{
	g_pBackground = g_pTextureManager->GetTexture("background");
	g_pCrosshair = g_pTextureManager->GetTexture("crosshair");
	Projectile::g_pBasicProjectileTexture = g_pTextureManager->GetTexture("basic_projectile");
	Pizza::g_pPizzaTexture = g_pTextureManager->GetTexture("pizza");
	PizzaSlice::g_pPizzaSliceTexture = g_pTextureManager->GetTexture("pizza_slice");
	Enemy::g_basicEnemySpriteSheet = g_pTextureManager->GetTexture("enemy_spritesheet");
	Enemy::g_Target = g_pPlayer;

	g_pMusic->Play(true);
	g_pMusic->SetVolume(MIX_MAX_VOLUME/4);
}

Level::~Level()
{
	delete g_pMusic;
	g_pMusic = nullptr;

	for (Projectile*& p : g_projectiles) {
		delete p;
		p = nullptr;
	}
	for (Enemy*& e : g_enemies) {
		delete e;
		e = nullptr;
	}

}

void Level::Update(float elapsedSec)
{

	g_pPlayer->Update(elapsedSec);



	UpdateEntities(elapsedSec);

	if (!coolDown) {
		SpawnWave(elapsedSec);
	}
	else {

		if (!(g_enemies.size() > 0)) {
			m_cooldownTimer += elapsedSec;
			if (m_cooldownTimer > m_coolDownDuration) {
				coolDown = false;
				m_waveTimer = 0;
				++m_waveCounter;
			}
		}

	}
}


void Level::Draw() const
{
	float width{ 50 }, height{ 50 };
	Rectf dst{ -width / 2.f,-height / 2.f ,width,height };
	glPushMatrix(); {
		const float bkgScale{ 0.5f };
		glScalef(bkgScale, bkgScale, 1.0f);
		g_pBackground->Draw();
	}
	glPopMatrix();

	glPushMatrix(); {
		const float chScale{ 0.5f };
		glTranslatef(mouseX, mouseY, 0.f);
		glScalef(chScale, chScale, 1.0f);
		g_pCrosshair->Draw(dst);
	}
	glPopMatrix();
	g_pPlayer->Draw();
	DrawEntities();
	DrawUI();
	if (!g_gameIsRunning) {
		DrawMenu();
	}
}

void Level::DrawUI() const
{
	{
		std::string message{ "Score : " };
		Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst{ 0,m_viewport.height - 50,80,50 };
		text.Draw(dst);
	}
	{
		//std::string message{ std::to_string(m_score) };
		//Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst{ 120,m_viewport.height - 50,80,50 };
		//text.Draw(dst);
		DrawNumber(m_score,6,dst);
	}


	{
		std::string message{ std::string{ "Health : " } };
		Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst{ m_viewport.width - 200,m_viewport.height - 50,80,50 };
		text.Draw(dst);
	}
	{
		Rectf dst{ m_viewport.width - 50,m_viewport.height - 50,30,50 };
		DrawNumber(g_pPlayer->GetHealth(), 3, dst);
	}

	{
		std::string message{ std::string("Wave : ") };
		Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst{ m_viewport.width / 2.f,m_viewport.height - 50,80,40 };
		text.Draw(dst);
	}
	{
		Rectf dst{ m_viewport.width/2.f + 80,m_viewport.height - 50,30,40 };
		DrawNumber(m_waveCounter,3,dst);
	}



	utils::SetColor(Color4f{ 0.2f,1.0f,0.1f,1.0f });

	{
		std::string message{ std::string("Ammo ") };
		Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst{ m_viewport.width - (g_pPlayer->g_pOven->m_maxAmmo) * 15 ,60,80,30 };
		text.Draw(dst);
	}
	utils::DrawRect(Rectf{ m_viewport.width - (g_pPlayer->g_pOven->m_maxAmmo) * 15 ,15,15.f * g_pPlayer->g_pOven->m_maxAmmo,30 });
	for (int i{}; i < g_pPlayer->g_pOven->m_currentAmmo;++i) {
		
		utils::FillRect(Rectf{ m_viewport.width - (i + 1) * 15 ,15,10,30 });
	}




	int length{6};
	const float spriteWidth{g_pTextureManager->GetTexture("numbers_sheet")->GetWidth()/5.0f};
	const float spriteHeight{g_pTextureManager->GetTexture("numbers_sheet")->GetHeight()/2.0f};


	const float scale{0.3f};
	for (int i{}; i < length; ++i) {
		Rectf dst{ 15.f +( spriteWidth * scale* i),15,spriteWidth* scale,spriteHeight* scale};
		Rectf src{ i*spriteWidth,spriteHeight*int(i/5),spriteWidth ,spriteHeight};
		g_pTextureManager->GetTexture("numbers_sheet")->Draw(dst,src);
	}
	utils::SetColor(Color4f{ 0.5f,0.8f,0.5f,1.0f });
	const float radius{60.f * scale};
	utils::DrawEllipse(Ellipsef{ Point2f{15+spriteWidth* scale * (g_pPlayer->g_pOven->m_projectileIndex+0.5f),15+spriteHeight* scale /2.0f},radius,radius },5.f);

	{
		std::string message{ std::string("Press NumKeys to toggle ") };
		Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst{ 15.f ,15 + spriteHeight * scale,160,40 };
		text.Draw(dst);
	}

}

void Level::DrawMenu() const
{
}

void Level::DrawNumber(int number, int maxNumberDisplaySize, const Rectf& dst) const
{
	const float dstNumberWidth{dst.width / maxNumberDisplaySize };
	for (int i{0}; i < maxNumberDisplaySize; ++i) {
		unsigned int n = unsigned int(number) % 10;
		number = number / 10.f;

		std::string message{ std::to_string(n) };
		Texture text(message, std::string("Nasa21.ttf"), 30, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf n_dst{ dst.left + (maxNumberDisplaySize - i)* dstNumberWidth,dst.bottom,dstNumberWidth,dst.height };
		text.Draw(n_dst);
	}
}

void Level::SpawnWave(float elapsedSec)
{
	const int maxEnemyCount{ 1 + 7* int(log(m_waveCounter+1)) };
	const float spawnCooldown{ 1.f + 1.f / m_waveCounter };
	const float m_waveDuration{ 5.0f + maxEnemyCount * spawnCooldown };
	m_waveTimer += elapsedSec;
	if (m_waveTimer > m_waveDuration) {
		coolDown = true;
		m_cooldownTimer = 0;
		m_enemyCounter = 0;
		return;
	}
	


	if (m_enemyCounter < maxEnemyCount) {
		if (m_waveTimer - spawnCooldown * g_enemies.size() > spawnCooldown) {

			int dir{ std::rand() % 2 + m_waveCounter%2 };
			Vector2f spawnPosition;
			switch (dir)
			{
			case 0:
				spawnPosition = Vector2f{ float(std::rand() % int(m_viewport.width)), 0 };
				break;
			case 1:
				spawnPosition = Vector2f{ 0,float(std::rand() % int(m_viewport.height)) };
				break;
			case 2:
				spawnPosition = Vector2f{ float(std::rand() % int(m_viewport.width)),m_viewport.height };
				break;
			case 3:
				spawnPosition = Vector2f{ m_viewport.width,float(std::rand() % int(m_viewport.height)) };
				break;
			default:
				break;
			}
			g_enemies.push_back(new Enemy(spawnPosition, std::rand() % 4 == 0, std::rand() % 4 == 0,
				float(std::rand() % 100 + 50 + m_waveCounter*1.5f),
				float(std::rand() % 5 + 1.f)/10.f,
				float(std::rand() % int(m_waveCounter) + m_waveCounter * 1.5f)));
			++m_enemyCounter;
		}
	}

}

std::list<Projectile*>& Level::GetProjectiles()
{
	return g_projectiles;
}

std::list<Enemy*>& Level::GetEnemies()
{
	return g_enemies;
}

void Level::UpdateEntities(float elapsedSec)
{

	for (Projectile*& p : g_projectiles) {
		if (p != nullptr) {
			p->Update(elapsedSec, g_enemies, g_projectiles);
		}
	}
	for (Enemy*& p : g_enemies) {
		if (p != nullptr) {
			p->Update(elapsedSec);
		}
	}
	for (auto itr = g_projectiles.begin();
		itr != g_projectiles.end();) {
		if (((*itr)->GetPosition().x < 0 or (*itr)->GetPosition().x > m_viewport.width) or
			((*itr)->GetPosition().y < 0 or (*itr)->GetPosition().y > m_viewport.height) or ((*itr)->GetHealth() <= 0)) {
			delete (*itr);
			(*itr) = nullptr;
			itr = g_projectiles.erase(itr);
		}
		else {
			++itr;
		}
	}
	for (auto itr = g_enemies.begin();
		itr != g_enemies.end();) {
		if ((*itr)->GetHealth() <= 0) {
			m_score += (*itr)->GetScore();
			delete (*itr);
			(*itr) = nullptr;
			itr = g_enemies.erase(itr);
			
		}
		else {
			++itr;
		}
	}
}

void Level::DrawEntities() const
{
	for (Projectile* p : g_projectiles) {
		p->Draw();
	}
	for (Enemy* e : g_enemies) {
		e->Draw();
	}
}


void Level::Reset()
{
	m_waveCounter = 1;
	g_pPlayer->SetHealth(10);
	g_pPlayer->SetPosition(Vector2f{ m_viewport.width / 2.f ,m_viewport.height / 2.f });
	m_score = 0;
	for (Enemy*& e : g_enemies) {
		delete e;
		e = nullptr;
	}
	for (Projectile*& e : g_projectiles) {
		delete e;
		e = nullptr;
	}
	g_enemies.clear();
	g_projectiles.clear();
}