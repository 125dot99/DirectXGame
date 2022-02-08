#include "EnemyManager.h"

#include "math/MyMath.h"
#include "math/MyRandom.h"
#include "MiniScreenData.h"

#include "Score.h"

void EnemyManager::CalcVelocityX()
{
	if (addVelocity.x < 0)
	{
		for (int x = 0; x < MAX_ENEMY_X; x++)
		{
			for (int y = 0; y < MAX_ENEMY_Y; y++)
			{
				auto pos = vec2_Enemys[y][x]->GetPosition();
				if (pos.x <= mini_screen::MIN_WALL.x)
				{
					addVelocity = Vector2(1, 1);
					return;
				}
			}
		}
	}
	else
	{
		for (int x = MAX_ENEMY_X - 1; x >= 0; x--)
		{
			for (int y = MAX_ENEMY_Y - 1; y >= 0; y--)
			{
				auto pos = vec2_Enemys[y][x]->GetPosition();
				if (pos.x >= mini_screen::MAX_WALL.x)
				{
					addVelocity = Vector2(-1, 1);
					return;
				}
			}
		}
	}
}

void EnemyManager::CalcVelocityY()
{
	if (addVelocity.y == 0)
	{
		return;
	}
	for (int y = 0; y < MAX_ENEMY_Y; y++)
	{
		for (int x = 0; x < MAX_ENEMY_X; x++)
		{
			auto pos = vec2_Enemys[y][x]->GetPosition();
			if (pos.y >= mini_screen::MAX_WALL.y - 120)
			{
				addVelocity.y = 0;
				return;
			}
		}
	}
}

void EnemyManager::MoveEnemys()
{
	if (moveFrameCount++ <= moveFrame)
	{
		return;
	}
	moveFrameCount = 0;
	CalcVelocityX();
	CalcVelocityY();
	animaIndex = animaIndex == 0 ? 1 : 0;
	Vector2 velocity = addVelocity * VELOCITY;
	if (addVelocity.y == 1)
	{
		velocity.x = 0;
	}
	for (int y = 0; y < MAX_ENEMY_Y; y++)
	{
		for (int x = 0; x < MAX_ENEMY_X; x++)
		{
			vec2_Enemys[y][x]->Move(velocity * Vector2(speed, 1));
		}
	}
	addVelocity.y = 0;
}

bool EnemyManager::RecursiveShot()
{
	int randIndexX = Random(0, MAX_ENEMY_X - 1);
	for (int y = MAX_ENEMY_Y - 1; y >= 0; y--)
	{
		auto enemy = vec2_Enemys[y][randIndexX];
		if (!enemy->IsDead())
		{
			for (int i = 0; i < MAX_ENEMY_BULLET; i++)
			{
				if (enemyBullets[i]->IsShot())
				{
					Vector2 pos = enemy->GetPosition();
					enemyBullets[i]->SetPosition(pos + Vector2(0, 6));
					enemyBullets[i]->Shot(Vector2::Up() * 4.0f);
					return true;
				}
			}
		}
	}
	return RecursiveShot();
}

void EnemyManager::AttackEnemys()
{
	if (shotFrameCount++ < shotFrame)
	{
		return;
	}
	shotFrameCount = 0;
	RecursiveShot();
}

EnemyManager::EnemyManager(HitPoint& hp) : hp(hp)
{
	speed = 1;
	animaIndex = 0;
	moveFrame = 20;
	shotFrame = 40;

	moveFrameCount = shotFrameCount = 0;
	addVelocity = Vector2(1, 0);

	hp.SetHP(MAX_ENEMY_Y * MAX_ENEMY_X);

	Vector2 offset(40.0f, 40.0f);
	Vector2 pos(mini_screen::CENTER.x - (offset.x + offset.x * (MAX_ENEMY_X / 4)), mini_screen::MIN_WALL.y + offset.y);
	
	vec2_Enemys.resize(MAX_ENEMY_Y);
	for (int i = 0; i < MAX_ENEMY_Y; i++)
	{
		int point = i == 1 || i == 2 ? ENEMY_SCORE_B : i == 3 || i == 4 ? ENEMY_SCORE_A : ENEMY_SCORE_C;
		vec2_Enemys[i].resize(MAX_ENEMY_X);
		for (int j = 0; j < MAX_ENEMY_X; j++)
		{
			vec2_Enemys[i][j] = new MiniEnemy(this, hp, point);
			vec2_Enemys[i][j]->SetPosition(pos + Vector2((float)j, (float)i) * offset);
		}
	}
	enemyBullets.resize(MAX_ENEMY_BULLET);
	for (int i = 0; i < MAX_ENEMY_BULLET; i++)
	{
		enemyBullets[i] = new Bullet();
	}
}

EnemyManager::~EnemyManager()
{
	vec2_Enemys.clear();
	enemyBullets.clear();
}

void EnemyManager::Update()
{
	//‘Ì—Í‚É‰ž‚¶‚Ä‘¬‚­‚·‚é
	float point = hp.GetHP();
	if (point < 10)
	{
		speed = (1.0f / 10.0f) * (10 - point) + 1.2f;
		moveFrame = (int)point;
		shotFrame = (int)point * 2 + 10;
	}
	MoveEnemys();
	AttackEnemys();
}

std::vector<std::vector<MiniEnemy*>> EnemyManager::GetEnemys() const
{
	return vec2_Enemys;
}

std::vector<Bullet*> EnemyManager::GetEnemyBullets() const
{
	return enemyBullets;
}

const int EnemyManager::GetAnimaIndex() const
{
	return animaIndex;
}