#include "MiniGameState.h"

#include "MiniScreenData.h"
#include "MiniPlayer.h"
#include "MiniEnemy.h"
#include "Bullet.h"
#include "MiniPlayerWall.h"

enum RESORCE_TEXTURE
{
	TEX_WHITE,
	TEX_ASCII_FONT,
	TEX_PLAYER,
	TEX_ENEMY_A,
	TEX_ENEMY_B,
	TEX_ENEMY_C,
	TEX_ENEMY_BULLET,
	TEX_TITLE,
	TEX_GAME_CLEAR,
	TEX_GAME_OVER,
	TEX_FRONT,
	TEX_FRONT_MASK,
	TEX_COLOR_RAMP_TITLE,
	TEX_COLOR_RAMP_PLAY,
	TEX_COLOR_RAMP_RESULT,
	TEX_MAX,
};

MiniTitle::MiniTitle(MiniGameScene* _owner) : owner(_owner)
{
	animaFrame = 0;
}

void MiniTitle::Update()
{
	const float t = 0.02f;
	animaFrame = Max(1.0f, animaFrame + t);
}

void MiniTitle::Draw()
{
	SpriteRenderer* spriteRenderer = owner->spriteRenderer.get();
	Vector4 color(1 * animaFrame);
	spriteRenderer->Draw(owner->textures[TEX_TITLE], owner->windowSize * Vector2(0.5f, 0), Vector2(0.5f, 0), color);
	
	Vector2 pos(owner->windowSize.x / 2, mini_screen::CENTER.y - 60);
	Vector2 pivot(0.5f, 0.5f);
	Vector2 unit(120, 0);
	Vector2 size(30, 30);
	Vector4 rect(0, 0, 12, 12);
	spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_A], pos - unit, size, rect, pivot, color);
	spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_B], pos, size, rect, pivot, color);
	spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_C], pos + unit, size, rect, pivot, color);

	SpriteFontAscii* spriteFontAscii = owner->spriteFontAscii.get();
	spriteFontAscii->Draw(EnemyManager::ENEMY_SCORE_A, pos - unit + Vector2(-15, 20), Vector2(1.4f), color);
	spriteFontAscii->Draw(EnemyManager::ENEMY_SCORE_B, pos + Vector2(-15, 20), Vector2(1.4f), color);
	spriteFontAscii->Draw(EnemyManager::ENEMY_SCORE_C, pos + unit + Vector2(-15, 20), Vector2(1.4f), color);
	spriteFontAscii->Draw("START", Vector2(mini_screen::CENTER.x + 40.0f, mini_screen::CENTER.y + 20), Vector2(2), color);
	spriteFontAscii->Draw("@ ??? 2022", Vector2(mini_screen::CENTER.x, mini_screen::MAX_WALL.y + 20), Vector2(2), color);
}

bool MiniTitle::IsState(const char* typeName) const
{
	return "Title" == typeName;
}

bool MiniTitle::IsNext() const
{
	return (Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B)) && 
		animaFrame == 1.0f;
}

MiniPlay::MiniPlay(MiniGameScene* _owner) : owner(_owner)
{
	playerBullet = Factory::Create<Bullet>();
	player = Factory::Create<MiniPlayer>(playerBullet, owner->playerHp);
	
	walls.resize(MAX_WALL_SIZE);
	wallHps.resize(MAX_WALL_SIZE);
	for (int i = 0; i < MAX_WALL_SIZE; i++)
	{
		walls[i] = Factory::Create<MiniPlayerWall>(wallHps[i]);
	}
	Vector2 startPos(mini_screen::MIN_WALL.x + 100.0f, mini_screen::MAX_WALL.y - 60);
	for (int i = 0; i < 4; i++)
	{
		walls[(UINT64)i * 3 + 0]->SetPosition(startPos + Vector2());
		walls[(UINT64)i * 3 + 1]->SetPosition(startPos + Vector2(20, 0));
		walls[(UINT64)i * 3 + 2]->SetPosition(startPos + Vector2(40, 0));
		startPos.x += 105;
	}
	enemyManager = Factory::CreateUnique<EnemyManager>(owner->enemyHp);
	objectManager = Factory::CreateUnique<ObjectManager>();
	objectManager->Add(player);
	objectManager->Add(playerBullet);

	Vector2 size(15, 15);
	collisionManager = Factory::CreateUnique<CollisionManager>();
	collisionManager->AddCollider(player, Factory::CreateShared<Box2DCollider>(Vector2::Zero(), size), 0b01);
	collisionManager->AddCollider(playerBullet, Factory::CreateShared<Box2DCollider>(Vector2::Zero(), Vector2(1.0f, size.y / 2)), 0b110);
	
	for (auto&& wall : walls)
	{
		objectManager->Add(wall);
		collisionManager->AddCollider(wall, Factory::CreateShared<Box2DCollider>(Vector2::Zero(), Vector2(10)), 0b110);
	}
	//マネージャーに渡す
	for (auto&& enemys : enemyManager->GetEnemys())
	{
		for (auto&& enemy : enemys)
		{
			objectManager->Add(enemy);
			collisionManager->AddCollider(enemy, Factory::CreateShared<Box2DCollider>(Vector2::Zero(), size), 0b010);
		}
	}
	for (auto&& eBullet : enemyManager->GetEnemyBullets())
	{
		objectManager->Add(eBullet);
		collisionManager->AddCollider(eBullet, Factory::CreateShared<Box2DCollider>(Vector2::Zero(), Vector2(2.0f, size.y / 2)), 0b1101);
	}
	objectManager->Initialize();
}

void MiniPlay::Update()
{
	//敵の移動、弾の処理
	enemyManager->Update();
	objectManager->Update();
	collisionManager->ChaeckAllCollisions();
}

void MiniPlay::Draw()
{
	SpriteRenderer* spriteRenderer = owner->spriteRenderer.get();

	const Vector2 pivot(0.5f, 0.5f);
	const Vector2 size(30.0f, 30.0f);
	spriteRenderer->Begin();
	//プレイヤー
	spriteRenderer->DrawExtend(owner->textures[TEX_PLAYER], player->GetPosition(), size, pivot);
	//プレイヤーの弾
	spriteRenderer->DrawExtend(owner->textures[TEX_WHITE], playerBullet->GetPosition(), Vector2(2, 15), pivot);
	//プレイヤーのHP
	{
		auto spriteFontAscii = owner->spriteFontAscii.get();
		auto playerHp = owner->playerHp.GetHP();
		Vector2 pos(mini_screen::MIN_WALL.x + 20, mini_screen::MAX_WALL.y + 20);
		spriteFontAscii->Draw(playerHp, pos, Vector2(2.2f));
		for (int i = 0; i < playerHp - 1; i++)
		{
			spriteRenderer->DrawExtend(owner->textures[TEX_PLAYER], pos + Vector2(40.0f * (float)(i + 1), 0), size);
		}
	}
	for (int i = 0; i < MAX_WALL_SIZE; i++)
	{
		float hp = wallHps[i].GetHP();
		Vector4 color((1.0f / 2.2f) * hp);
		spriteRenderer->DrawExtend(owner->textures[TEX_WHITE], walls[i]->GetPosition(), Vector2(20), pivot, color);
	}
	//敵
	auto enemys = enemyManager->GetEnemys();
	const Vector4 rectSize = Vector4(0, 0, 12, 12) + Vector4((float)enemyManager->GetAnimaIndex() * 12);
	for (int i = 0; i < EnemyManager::MAX_ENEMY_X; i++)
	{
		spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_C], enemys[0][i]->GetPosition(), size, rectSize, pivot);
		spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_B], enemys[1][i]->GetPosition(), size, rectSize, pivot);
		spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_B], enemys[2][i]->GetPosition(), size, rectSize, pivot);
		spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_A], enemys[3][i]->GetPosition(), size, rectSize, pivot);
		spriteRenderer->DrawExtendRect(owner->textures[TEX_ENEMY_A], enemys[4][i]->GetPosition(), size, rectSize, pivot);
	}
	auto eBullets = enemyManager->GetEnemyBullets();
	for (int i = 0; i < EnemyManager::MAX_ENEMY_BULLET; i++)
	{
		spriteRenderer->DrawExtend(owner->textures[TEX_ENEMY_BULLET], eBullets[i]->GetPosition(), Vector2(12.0f, 18.0f), pivot);
	}
}

bool MiniPlay::IsState(const char* typeName) const
{
	return "Play" == typeName;
}

bool MiniPlay::IsNext() const
{
	return owner->playerHp.IsDead() || owner->enemyHp.IsDead();
}

MiniGameClear::MiniGameClear(MiniGameScene* _owner) : owner(_owner)
{

}

void MiniGameClear::Update()
{

}

void MiniGameClear::Draw()
{
	SpriteRenderer* spriteRenderer = owner->spriteRenderer.get();
	spriteRenderer->Draw(owner->textures[TEX_GAME_CLEAR], owner->windowSize * Vector2(0.5f, 0), Vector2(0.5f, 0));
}

bool MiniGameClear::IsState(const char* typeName) const
{
	return "GameClear" == typeName;
}

bool MiniGameClear::IsNext() const
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B);
}

MiniGameOver::MiniGameOver(MiniGameScene* _owner) : owner(_owner)
{

}

void MiniGameOver::Update()
{

}

void MiniGameOver::Draw()
{
	SpriteRenderer* spriteRenderer = owner->spriteRenderer.get();
	spriteRenderer->Draw(owner->textures[TEX_GAME_OVER], owner->windowSize * Vector2(0.5f, 0), Vector2(0.5f, 0));
}

bool MiniGameOver::IsState(const char* typeName) const
{
	return "GameOver" == typeName;
}

bool MiniGameOver::IsNext() const
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B);
}
