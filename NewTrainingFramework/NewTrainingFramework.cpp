// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "EffectManager.h"


int Init ( ESContext *esContext )
{

	ResourceManager::GetInstance()->Init("../Resources/RM.txt");
	SceneManager::GetInstance()->Init("../Resources/SM.txt");
	EffectManager::GetInstance()->Init("../Resources/EM.txt");

	glClearColor (0.690f, 0.878f, 0.902f, .0f);
	
	return 0;

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (EffectManager::GetInstance()->GetEffect() != EEffect::NONE)
	{
		EffectManager::GetInstance()->RenderEffect();
	}
	else
	{
		SceneManager::GetInstance()->Render();
	}
	
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	SceneManager::GetInstance()->Update(deltaTime);
}

void Key(ESContext *esContext, unsigned char key, bool bIsPressed)
{
	InputManager::GetInstance()->UpdateKeyPress(key, bIsPressed);
}

void CleanUp()
{
	ResourceManager::GetInstance()->CleanUp();
	SceneManager::GetInstance()->CleanUp();
	EffectManager::GetInstance()->CleanUp();
}

//////////////////////////////////////////// WINDOWS main function ////////////////////////////////////////////////////
#ifdef _WIN32

#include <conio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	printf("Press any key...\n");
	_getch();

	return 0;
}

#endif
//////////////////////////////////////////// WINDOWS main function ////////////////////////////////////////////////////
