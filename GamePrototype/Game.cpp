#include "pch.h"
#include "Game.h"
#include "iostream"
#include "string"

#include "utils.h"


#include "Player.h"
#include "Oven.h"
#include "Level.h"
#include "TextureManager.h"


#include "Projectile.h"
#include "Pizza.h"
#include "PizzaSlice.h"
#include "FireParticle.h"
#include "GravityOrb.h"

bool g_gameIsRunning{ true };
bool g_defeated{false};


float distriburtion(float x, float y) {
	return expf(-x * x - y * y);
}

void BasicShootingFunction(Projectile* prjc, std::list<Projectile*>& projectileList, Player* parent) {


	Projectile* p{ prjc->Clone() };
	float angle{ parent->GetAngle() };
	Vector2f position{ parent->GetPosition() };

	const float ProjectileSpeed{ (parent->g_pOven->m_projectileEjectionForce + prjc->GetProjectileSpeed())/2.0f};
	const float maxAngVel{ 500 };
	const float minAngVel{ 20 };
	const float knockbackPercentage{0.3f};
	const float knockback{ ProjectileSpeed * knockbackPercentage };
	//Projectile* p{ new Pizza(m_Position, Vector2f{cos((m_angle + 90) * 3.14f / 180.f),sin((m_angle + 90) * 3.14f / 180.f)} *ProjectileSpeed,m_angle + 90,this) };
	p->SetPosition(position);
	p->SetVelocity(Vector2f{ cos((angle + 90) * 3.14f / 180.f),sin((angle + 90) * 3.14f / 180.f) } *ProjectileSpeed);
	p->SetAngle(angle + 90);
	p->SetAngularVelocity(std::rand() % int(maxAngVel - minAngVel) + minAngVel);
	projectileList.push_back(p);


	parent->SetVelocity(parent->GetVelocity() - Vector2f{cos((angle + 90) * 3.14f / 180.f), sin((angle + 90) * 3.14f / 180.f)} *knockback);

}
void FireShootingFunction(Projectile* prjc, std::list<Projectile*>& projectileList, Player* parent) {


	const int amountBurstParticle{10};
	float angle{ parent->GetAngle() + 90 };
	const Vector2f offset{0.f,0.5f};
	Vector2f spawnPosition{ parent->GetPosition() + offset };



	for (int i{}; i < amountBurstParticle; ++i) {
		Projectile* p{ prjc->Clone() };
		//Projectile* p{ new FireParticle()};
		
		const float ProjectileSpeed{ float(std::rand() % 200 + 400)};
		const float maxAngVel{ 150 };
		const float minAngVel{ -150 };

		const int maxAngleOffset{10};
		const float angleOffset{ float(std::rand() % maxAngleOffset - maxAngleOffset/2) };
		const Vector2f spawnOffset{ float(std::rand() % 10 - 5),0.1f };
		const float knockbackPercentage{ 0.001f };
		const float knockback{ ProjectileSpeed * knockbackPercentage };
		//Projectile* p{ new Pizza(m_Position, Vector2f{cos((m_angle + 90) * 3.14f / 180.f),sin((m_angle + 90) * 3.14f / 180.f)} *ProjectileSpeed,m_angle + 90,this) };
		p->SetPosition(spawnPosition+ spawnOffset);
		p->SetVelocity(Vector2f{ cos((angle + angleOffset) * 3.14f / 180.f),sin((angle + angleOffset) * 3.14f / 180.f) } * ProjectileSpeed);
		p->SetAngle(angle + angleOffset);
		p->SetAngularVelocity(std::rand() % int(maxAngVel - minAngVel) + minAngVel);
		const float angleOffsetPercentage{1- std::abs(angleOffset) / maxAngleOffset };
		p->SetLifeTime(((std::rand() % int((Projectile::maxLifeTime - Projectile::minLifeTime)*10))/10.f + Projectile::minLifeTime)*(angleOffsetPercentage* angleOffsetPercentage));
		projectileList.push_back(p);
		parent->SetVelocity(parent->GetVelocity() - Vector2f{ cos((angle) * 3.14f / 180.f), sin((angle) * 3.14f / 180.f) } *knockback);
	}
	

}
Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
}


void Game::Initialize( )
{

	SDL_SetWindowGrab(SDL_GL_GetCurrentWindow(),SDL_TRUE);
	//SDL_CaptureMouse(SDL_TRUE);

	g_shootingFunctionLibrary.push_back(BasicShootingFunction);
	g_shootingFunctionLibrary.push_back(FireShootingFunction);
	std::srand(std::time(nullptr));
	g_pTextureManager = new TextureManager();
	g_pTextureManager->AddTexture("Background.jpg",			"background");
	g_pTextureManager->AddTexture("Background.jpg",			"backgroundcopy");
	g_pTextureManager->AddTexture("CrossHair.png",			"crosshair");
	g_pTextureManager->AddTexture("Bread.png",				"breadd");
	g_pTextureManager->AddTexture("BasicProjectile.png",	"basic_projectile");
	g_pTextureManager->AddTexture("Pizza.png",				"pizza");
	g_pTextureManager->AddTexture("PizzaSlice.png",			"pizza_slice");

	g_pTextureManager->AddTexture("Numbers.png",			"numbers_sheet");
	g_pTextureManager->AddTexture("Oven1.png",				"oven_spritesheet");
	g_pTextureManager->AddTexture("EnemySpriteSheet1.png",	"enemy_spritesheet");
	g_pTextureManager->AddTexture("MuzzleFlash.png",		"muzzleflash_spritesheet");
	g_pTextureManager->AddTexture("CartoonExplosion.png",	"explosion_spritesheet");
	g_pTextureManager->AddTexture("Orb.png",	"orb");
	g_pTextureManager->AddTexture("Orb1.png",	"orb1");


	g_pOven1 = new Oven(20, 10, 0,0,
		g_pTextureManager->GetTexture("oven_spritesheet"),
		g_pTextureManager->GetTexture("muzzleflash_spritesheet"));

	g_pPlayer = new Player(Vector2f{ GetViewPort().width / 2.f ,GetViewPort().height / 2.f }, Vector2f{}, g_pOven1);
	g_pLevel = new Level(GetViewPort(), g_pPlayer, g_pTextureManager);

	Entity::g_pLevel = g_pLevel;
	Projectile::g_pBasicProjectileTexture = g_pTextureManager->GetTexture("breadd");
	Pizza::g_pPizzaTexture = g_pTextureManager->GetTexture("pizza");
	PizzaSlice::g_pPizzaSliceTexture = g_pTextureManager->GetTexture("pizza_slice");
	GravityOrb::g_orbTexture = g_pTextureManager->GetTexture("orb1");
	GravityOrb::g_pDisplayedBackground = g_pTextureManager->GetTexture("background");
	GravityOrb::g_pReferencBackground = g_pTextureManager->GetTexture("backgroundcopy");
	GravityOrb::g_viewPort = GetViewPort();
	Oven::g_projectileLibrary = &g_ProjectileSamples;
	Oven::g_shootingLibrary = &g_shootingFunctionLibrary;

	g_ProjectileSamples.push_back(new Projectile	(Vector2f{}, Vector2f{}, 0, g_pPlayer));
	g_ProjectileSamples.push_back(new PizzaSlice	(Vector2f{}, Vector2f{}, 0, g_pPlayer));
	g_ProjectileSamples.push_back(new Pizza			(Vector2f{}, Vector2f{}, 0, g_pPlayer));
	g_ProjectileSamples.push_back(new FireParticle	(Vector2f{}, Vector2f{}, 0, g_pPlayer));
	g_ProjectileSamples.push_back(new GravityOrb	(Vector2f{}, Vector2f{}, 0, g_pPlayer,12000, -10000));
	g_ProjectileSamples.push_back(new GravityOrb	(Vector2f{}, Vector2f{}, 0, g_pPlayer,-12000, 0));


}

void Game::Cleanup( )
{
	delete g_pPlayer;
	delete g_pLevel;

}

void Game::Update(float elapsedSec)
{
	if (!g_gameIsRunning) {
		return;
	}
	if (g_pPlayer->GetHealth() <= 0) {
		g_gameIsRunning = false;
		g_defeated = true;
	}
	float count{200};


	g_pLevel->Update(elapsedSec);

}

void Game::Draw( ) const
{
	ClearBackground();
	g_pLevel->Draw();

	
	if (g_defeated) {
		Texture EndScreen("DEFEATED Press 0 to restart", std::string("Nasa21.ttf"), 20, Color4f{ 0.2f,1.0f,0.1f,1.0f });
		Rectf dst = Rectf{ 0,100,700,100 };
		EndScreen.Draw(dst);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (e.keysym.sym == SDLK_ESCAPE) {
		//SDL_CaptureMouse(SDL_FALSE);
		SDL_SetWindowGrab(SDL_GL_GetCurrentWindow(), SDL_FALSE);
		g_gameIsRunning = false;
	}

	if (e.keysym.sym == SDLK_0)
	{
		if (!g_gameIsRunning && g_defeated) {
			g_pLevel->Reset();
			g_gameIsRunning = true;
			g_defeated = false;

		}
		if (!g_gameIsRunning && !g_defeated) {
			SDL_SetWindowGrab(SDL_GL_GetCurrentWindow(), SDL_TRUE);
			g_gameIsRunning = true;
		}
	}


	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}
void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	Level::mouseX = float(e.x);
	Level::mouseY = float(e.y);
	g_pPlayer->HandleMouseInput();
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{


	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		Level::leftMouse = true;
		g_pPlayer->Shoot();
		break;
	case SDL_BUTTON_RIGHT:
		Level::rightMouse = true;
		break;
	case SDL_BUTTON_MIDDLE:
		std::cout << " middle button " << std::endl;
		break;
	default:
	
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		Level::leftMouse = false;
		break;
	case SDL_BUTTON_RIGHT:
		Level::rightMouse = false;
		break;
	case SDL_BUTTON_MIDDLE:
		std::cout << " middle button " << std::endl;
		break;
	default:

		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
