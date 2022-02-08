#pragma once
#include <GameLibHeader.h>

#include "MiniGameScene.h"
#include "EnemyManager.h"

using namespace gamelib;
//ミニゲームのタイトルシーン
class MiniTitle : public BaseChildScene
{
private:
	float animaFrame;
	MiniGameScene* owner;
public:
	MiniTitle(MiniGameScene* _owner);
	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};

//ミニゲームのプレイシーン
class MiniPlay : public BaseChildScene
{
private:
	MiniGameScene* owner;

	GameObject* player;
	GameObject* playerBullet;

	static const int MAX_WALL_SIZE = 12;
	std::vector<GameObject*> walls;
	std::vector<HitPoint> wallHps;
	std::unique_ptr<EnemyManager> enemyManager;
	std::unique_ptr<CollisionManager> collisionManager;
	std::unique_ptr<ObjectManager> objectManager;
public:
	MiniPlay(MiniGameScene* _owner);
	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};

class MiniGameClear : public BaseChildScene
{
private:
	MiniGameScene* owner;
public:
	MiniGameClear(MiniGameScene* _owner);
	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};

class MiniGameOver : public BaseChildScene
{
private:
	MiniGameScene* owner;
public:
	MiniGameOver(MiniGameScene* _owner);
	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};