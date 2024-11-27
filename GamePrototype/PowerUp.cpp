#include "pch.h"
#include "PowerUp.h"
#include "Level.h"

void PowerUp::Update(float elapsedSec)
{
	if (m_Position.x < 0 or m_Position.x > g_pLevel->m_viewport.width) {
		m_Velocity = Vector2f{ -m_Velocity.x,m_Velocity.y };
	}
	if (m_Position.y < 0 or m_Position.y > g_pLevel->m_viewport.height) {
		m_Velocity = Vector2f{ m_Velocity.x,-m_Velocity.y };
	}
	m_Position = Vector2f{
		utils::Clamp(0, g_pLevel->m_viewport.width, m_Position.x),
		utils::Clamp(0, g_pLevel->m_viewport.height, m_Position.y)
	};
	Entity::Update(elapsedSec);
}
