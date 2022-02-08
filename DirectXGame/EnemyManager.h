#pragma once
#include "Bullet.h"
#include "MiniEnemy.h"
#include "gametemp/HitPoint.h"
#include <vector>

//�G�̊Ǘ��N���X
class EnemyManager
{
public:
	static const int MAX_ENEMY_X = 11;
	static const int MAX_ENEMY_Y = 5;
	static const int MAX_ENEMY_BULLET = 10;

	static const int ENEMY_SCORE_A = 100;
	static const int ENEMY_SCORE_B = 200;
	static const int ENEMY_SCORE_C = 300;
private:
	//�ړ��t���[��
	int moveFrame;
	int moveFrameCount;

	//�U���t���[��
	int shotFrame;
	int shotFrameCount;
	//�A�j���[�V����
	int animaIndex;

	float speed;

	Vector2 addVelocity;
	const Vector2 VELOCITY = Vector2(5.0f, 40.0f);
	std::vector<Bullet*> enemyBullets;
	std::vector<std::vector<MiniEnemy*>> vec2_Enemys;

	//�G�̐�
	HitPoint& hp;
	//���x�����߂�
	void CalcVelocityX();
	void CalcVelocityY();
	//�ړ�����
	void MoveEnemys();
	//�U������
	bool RecursiveShot();
	void AttackEnemys();
public:
	EnemyManager(HitPoint& hp);
	~EnemyManager();
	
	void Update();

	std::vector<std::vector<MiniEnemy*>> GetEnemys() const;
	std::vector<Bullet*> GetEnemyBullets() const;
	const int GetAnimaIndex() const;
};

