#include "stdafx.h"
#include "InputManager.h"
#include "Globals.h"
#include "EffectManager.h"

InputManager* InputManager::s_Intance = nullptr;

InputManager::InputManager() :m_KeyPressing(0)
{

}

InputManager::~InputManager()
{
	delete s_Intance;
}

InputManager* InputManager::GetInstance()
{
	if (!s_Intance)
	{
		s_Intance = new InputManager();
	}
	return s_Intance;
}

void InputManager::UpdateKeyPress(unsigned char key, bool bIsPressed)
{
	printf("Key press %c\n", key);
	if (bIsPressed)
	{
		switch (key)
		{
		case 'D':
		{
			m_KeyPressing |= EMove::RIGHT;
		}break;
		case 'S':
		{
			m_KeyPressing |= EMove::BACKWARD;
		}break;
		case 'A':
		{
			m_KeyPressing |= EMove::LEFT;
		}break;
		case 'W':
		{
			m_KeyPressing |= EMove::FORWARD;
		}break;
		case 'Q':
		{
			m_KeyPressing |= EMove::UP;
		}break;
		case 'E':
		{
			m_KeyPressing |= EMove::DOWN;
		}break;
		case '(':
		{
			m_KeyPressing |= EMove::TURNDOWN;
		}break;
		case '&':
		{
			m_KeyPressing |= EMove::TURNUP;
		}break;
		case '%':
		{
			m_KeyPressing |= EMove::TURNRIGHT;
		}break;
		case '\'':
		{
			m_KeyPressing |= EMove::TURNLEFT;
		}break;
		case 'p':
		{
			EffectManager::GetInstance()->SetEffect(EEffect::NONE);
			//enableEffect = false;
		}break;
		case 'q':
		{
			EffectManager::GetInstance()->SetEffect(EEffect::BW);
			//enableEffect = true;
			//effectName = "BW";
		}break;
		case 'r':
		{
			EffectManager::GetInstance()->SetEffect(EEffect::BLUR);
			//enableEffect = true;
			//effectName = "Blur";
		}break;
		case 's':
		{
			EffectManager::GetInstance()->SetEffect(EEffect::BLOOM);
			//enableEffect = true;
			//effectName = "Bloom";
		}break;
		default:
			break;
		}
	}
	else
	{

		switch (key)
		{
		case 'D':
		{
			m_KeyPressing &= ~EMove::RIGHT;
		}break;
		case 'S':
		{
			m_KeyPressing &= ~EMove::BACKWARD;
		}break;
		case 'A':
		{
			m_KeyPressing &= ~EMove::LEFT;
		}break;
		case 'W':
		{
			m_KeyPressing &= ~EMove::FORWARD;
		}break;
		case 'Q':
		{
			m_KeyPressing &= ~EMove::UP;
		}break;
		case 'E':
		{
			m_KeyPressing &= ~EMove::DOWN;
		}break;
		case '(':
		{
			m_KeyPressing &= ~EMove::TURNDOWN;
		}break;
		case '&':
		{
			m_KeyPressing &= ~EMove::TURNUP;
		}break;
		case '%':
		{
			m_KeyPressing &= ~EMove::TURNRIGHT;
		}break;
		case '\'':
		{
			m_KeyPressing &= ~EMove::TURNLEFT;
		}break;
		default:
			break;
		}
	}
}

unsigned int InputManager::GetKeyPressing()
{
	return m_KeyPressing;
}