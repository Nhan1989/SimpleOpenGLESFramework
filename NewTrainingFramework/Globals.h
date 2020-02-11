#pragma once
#include <string>
class Globals
{
public:
	static int screenWidth;
	static int screenHeight;
};
enum EMove
{
	LEFT		= 1 << 0,
	RIGHT		= 1 << 1,
	FORWARD		= 1 << 2,
	BACKWARD	= 1 << 3,
	TURNRIGHT	= 1 << 4,
	TURNLEFT	= 1 << 5,
	TURNUP		= 1 << 6,
	TURNDOWN	= 1 << 7,
	UP			= 1 << 8,
	DOWN		= 1 << 9,
	IDLE		= 1 << 10
};

enum class ObjectType
{
	CHARACTER,
	LIGHT,
	TERRAINS,
	REFLECTBALL,
	CUBEMAP,
	FIRE,
	WATER,
	NONE
};

struct CameraCreationSetting
{
	float f_near;
	float f_far;
	float fov;
	float speed;
};


const float quadPositions[] = { 1.0,  1.0, 0.0,
-1.0,  1.0, 0.0,
-1.0, -1.0, 0.0,
-1.0, -1.0, 0.0,
1.0, -1.0, 0.0,
1.0,  1.0, 0.0 };
const float quadTexcoords[] = { 1.0, 1.0,
0.0, 1.0,
0.0, 0.0,
0.0, 0.0,
1.0, 0.0,
1.0, 1.0 };

#define ReturnIf(x, ...) do{ if(x){ return __VA_ARGS__; } }while(false)
#define ReturnUnless(x, ...) do{ if(!(x)){ return __VA_ARGS__; } }while(false)

#define SAFE_DEL(x) delete x;\
					x = nullptr;

#define SAFE_DEL_ARRAY(x) delete[] x;\
							x = nullptr;
