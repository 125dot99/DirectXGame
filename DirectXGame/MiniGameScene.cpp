#include "MiniGameScene.h"

#include "CafeScene.h"
#include "MiniScreenData.h"

#include "InvaderFilter.h"
#include "MiniGameState.h"
#include "SceneFade.h"

#include "Score.h"
#include "loader/UtilityString.h"

static int hiScore;

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

void MiniGameScene::Initialize()
{
	auto resourceManager = ResourceManager::GetInstance();
	textures.resize(TEX_MAX);
	textures[TEX_WHITE] = resourceManager->GetDefalutTexture();
	textures[TEX_ASCII_FONT] = resourceManager->GetTexture("ascii_retro_8x12.png");
	textures[TEX_PLAYER] = resourceManager->GetTexture("player.png");
	textures[TEX_ENEMY_A] = resourceManager->GetTexture("enemy1.png");
	textures[TEX_ENEMY_B] = resourceManager->GetTexture("enemy2.png");
	textures[TEX_ENEMY_C] = resourceManager->GetTexture("enemy3.png");
	textures[TEX_ENEMY_BULLET] = resourceManager->GetTexture("enemy_bullet.png");
	textures[TEX_TITLE] = resourceManager->GetTexture("game_table_title.png");
	textures[TEX_GAME_CLEAR] = resourceManager->GetTexture("game_clear.png");
	textures[TEX_GAME_OVER] = resourceManager->GetTexture("game_over.png");
	textures[TEX_FRONT] = resourceManager->GetTexture("game_table_front.png");
	textures[TEX_FRONT_MASK] = resourceManager->GetTexture("game_table_front_mask.png");
	textures[TEX_COLOR_RAMP_TITLE] = resourceManager->GetTexture("color_ramp_title.png");
	textures[TEX_COLOR_RAMP_PLAY] = resourceManager->GetTexture("color_ramp_play.png");
	textures[TEX_COLOR_RAMP_RESULT] = resourceManager->GetTexture("color_ramp_result.png");
	
	filter = Factory::CreateUnique<InvaderFilter>(textures[TEX_COLOR_RAMP_TITLE]);
	
	windowSize = Vector2(1280, 720);
	spriteRenderer = Factory::CreateUnique<SpriteRenderer>();
	spriteRenderer->SetScreenSize(windowSize);

	spriteFontAscii = Factory::CreateUnique<SpriteFontAscii>(spriteRenderer.get(), textures[TEX_ASCII_FONT]);
	
	strHiScore = utility_string::LoadTextFile("Resources/Score/score.sts");
	if (strHiScore.size() == 0)
	{
		strHiScore = "00000";
	}
	//hpの初期化
	playerHp.SetHP(3);
	Score::Set(0);

	childScene = Factory::CreateUnique<MiniTitle>(this);
}

void MiniGameScene::Update()
{
	const float t = 0.02f;
	animaFrame = Max(1.0f, animaFrame + t);
	childScene->Update();
	if (childScene->IsNext())
	{
		if (childScene->IsState("Title"))
		{
			childScene = Factory::CreateUnique<MiniPlay>(this);
			filter = Factory::CreateUnique<InvaderFilter>(textures[TEX_COLOR_RAMP_PLAY]);
		}
		else
		if (childScene->IsState("Play"))
		{
			filter = Factory::CreateUnique<InvaderFilter>(textures[TEX_COLOR_RAMP_RESULT]);
			if (enemyHp.IsDead())
			{
				childScene = Factory::CreateUnique<MiniGameClear>(this);
			}
			else if(playerHp.IsDead())
			{
				childScene = Factory::CreateUnique<MiniGameOver>(this);
			}
		}
		else
		if (childScene->IsState("GameClear"))
		{
			childScene = Factory::CreateUnique<MiniPlay>(this);
			filter = Factory::CreateUnique<InvaderFilter>(textures[TEX_COLOR_RAMP_PLAY]);
			playerHp.SetHP(playerHp.GetHP() + 1.0f);
		}
		else
		if (childScene->IsState("GameOver"))
		{
			childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
		}
		else
		if (childScene->IsState("FadeIn"))
		{
			nextSceneState = true;
		}
	}
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::Z) || 
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::A))
	{
		childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
	}
}

void MiniGameScene::Draw()
{
	spriteRenderer->Begin();
	if (!childScene->IsState("FadeIn"))
	{
		childScene->Draw();
	}
	Vector4 color(animaFrame);
	Vector2 mp = mini_screen::MIN_WALL;
	spriteFontAscii->Draw("SCORE1", mp + Vector2(0, -60), Vector2(2), color);
	std::string scoreStr("00000");
	int score = Score::GetScore();
	if (score > 0)
	{
		std::string str = std::to_string(score);
		scoreStr.erase(0, str.size());
		scoreStr.insert(5 - str.size(), str);
	}
	spriteFontAscii->Draw(scoreStr, mp + Vector2(0, -30), Vector2(2), color);
	spriteFontAscii->Draw("HI - SCORE", Vector2(mini_screen::CENTER.x, mp.y + -60), Vector2(2), color);
	spriteFontAscii->Draw(strHiScore, Vector2(mini_screen::CENTER.x + 40, mp.y + -30), Vector2(2), color);
	spriteRenderer->Draw(textures[TEX_FRONT_MASK], Vector2::Zero(), color);
	
	//ブラウン管フィルター
	filter->After();
	
	spriteRenderer->Begin();
	spriteRenderer->Draw(textures[TEX_FRONT], Vector2::Zero());
	if (childScene->IsState("FadeIn"))
	{
		childScene->Draw();
	}
	spriteRenderer->End();
}

void MiniGameScene::Finalize()
{
	if (std::atoi(strHiScore.c_str()) < Score::GetScore())
	{
		std::string scoreStr("00000");
		std::string str = std::to_string(Score::GetScore());
		scoreStr.erase(0, str.size());
		scoreStr.insert(5 - str.size(), str);
		utility_string::SaveText("Resources/Score/score.sts", scoreStr);
	}
	textures.clear();
}

void MiniGameScene::NextScene(ISceneManager* pSceneManager)
{
	pSceneManager->ChangeScene(new CafeScene);
}
