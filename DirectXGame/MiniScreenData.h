#pragma once
#include "math/Vector.h"

using namespace gamelib;
namespace mini_screen
{
//1280, 720
static const Vector2 MIN_WALL(360.0f, 120.0f);
static const Vector2 MAX_WALL(920.0f, 600.0f);
static const Vector2 CENTER(MAX_WALL - MIN_WALL);
};
