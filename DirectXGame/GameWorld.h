#pragma once
#include "math/Vector.h"

#include "collision/CollisionPrimitive.h"

using namespace gamelib;
namespace game_world
{
static const float MAP_RADIAS = 50.0f;
static const float MAP_CIRCLE_WALL_RADIAS = 35.0f;
static const Vector2 MAP_SIZE(MAP_RADIAS * 2);
static const Vector2 WAVE_TEXTURE_SIZE(512, 512);

static const primitive::Box BOX_OUT_CAFE{ Vector3(-10.4f, -1, 19.4f), Vector3(10.4f, 5, 30.6f) };

//“X“à‚Ì‹éŒ`”»’è—p
static const primitive::Box BOX_IN_CAFE{ Vector3(-5, -1, -1), Vector3(5, 3, 3) };
static const primitive::Box BOX_IN_CAFE_TABLE_FRONT{ Vector3(0.75f, -1, -2), Vector3(6, 1, 1.3f) };
static const primitive::Box BOX_IN_CAFE_TABLE_BACK{ Vector3(0.75f, -1, 1.5f), Vector3(6, 1, 4) };
} // namespace game_world