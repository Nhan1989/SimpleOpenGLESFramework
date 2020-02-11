#pragma once

class InputManager
{
public:
	static InputManager* GetInstance();

	void UpdateKeyPress(unsigned char key, bool bIsPressed);
	unsigned int GetKeyPressing();
private:
	InputManager();
	~InputManager();
	static InputManager* s_Intance;

	unsigned int m_KeyPressing;

};