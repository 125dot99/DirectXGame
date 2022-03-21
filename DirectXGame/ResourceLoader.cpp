#include "ResourceLoader.h"

#include "loader/ResourceManager.h"
#include "pipeline/PipelineManager.h"
#include "dx12/Material.h"

using namespace gamelib;
void resources_loader::Load()
{
	static bool loadFlag = false;
	if (loadFlag)
	{
		return;
	}
	PipelineManager::GetInstance()->Initialize();

	const std::string PATH = "Resources/Texture/";
	const auto& resourceManager = ResourceManager::GetInstance();
	//テクスチャ読み込み
	resourceManager->LoadTextureFromFile(PATH + "ascii_myfont_8x12.png");
	resourceManager->LoadTextureFromFile(PATH + "font_jp_32x32.png");
	resourceManager->LoadTextureFromFile(PATH + "font_retro_10x8.png");
	resourceManager->LoadTextureFromFile(PATH + "back.png");
	resourceManager->LoadTextureFromFile(PATH + "char_event.png");
	resourceManager->LoadTextureFromFile(PATH + "char_event_exit.png");
	resourceManager->LoadTextureFromFile(PATH + "char_event_game.png");
	resourceManager->LoadTextureFromFile(PATH + "title.png");
	resourceManager->LoadTextureFromFile(PATH + "op_ui.png");
	resourceManager->LoadTextureFromFile(PATH + "fotmode.png");
	resourceManager->LoadTextureFromFile(PATH + "noon_and_night.png");

	resourceManager->LoadTextureFromFile(PATH + "pause_camera.png");
	resourceManager->LoadTextureFromFile(PATH + "pause_back.png");
	resourceManager->LoadTextureFromFile(PATH + "pause_end.png");

	resourceManager->LoadTextureFromFile(PATH + "filter_none.png");
	resourceManager->LoadTextureFromFile(PATH + "filter_gray.png");
	resourceManager->LoadTextureFromFile(PATH + "filter_sepia.png");
	resourceManager->LoadTextureFromFile(PATH + "filter_mosaic.png");
	resourceManager->LoadTextureFromFile(PATH + "filter_grain.png");
	resourceManager->LoadTextureFromFile(PATH + "filter_blur.png");
	resourceManager->LoadTextureFromFile(PATH + "filter_bloom.png");

	//Particleフォルダ
	resourceManager->LoadTextureFromFile(PATH + "Particle/effect1.png");
	
	//2dフォルダ
	resourceManager->LoadTextureFromFile(PATH + "2D/player.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/enemy1.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/enemy2.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/enemy3.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/enemy_bullet.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/color_ramp_title.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/color_ramp_play.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/color_ramp_result.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/game_table_front.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/game_table_front_mask.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/game_table_title.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/game_clear.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/game_over.png");
	resourceManager->LoadTextureFromFile(PATH + "2D/ascii_retro_8x12.png");

	//モデル読み込み
	resourceManager->LoadModelFromFile("ship3");
	resourceManager->LoadModelFromFile("char");
	resourceManager->LoadModelFromFile("cafe_exterior");
	resourceManager->LoadModelFromFile("circle_wall");
	resourceManager->LoadModelFromFile("hemisphere");
	resourceManager->LoadModelFromFile("sphere");
	
	resourceManager->LoadModelFromFile("mistress");
	resourceManager->LoadModelFromFile("sofa");
	resourceManager->LoadModelFromFile("table2");
	resourceManager->LoadModelFromFile("game_table");
	resourceManager->LoadModelFromFile("cafe_interior");
	
	//音声データ読み込み
	resourceManager->LoadSoundFromFile("mini_se_ok.wav");

	//Fbxのマテリアルは直接書き換えられないのでマテリアルを変更
	PhongMaterial material;
	material.ambient = Vector3(0.85f, 0.85f, 0.85f);
	material.diffuse = Vector3(0.3f, 0.3f, 0.3f);
	material.specular = Vector3::Zero();
	auto fbxMtl1 = std::dynamic_pointer_cast<PhongMaterial>(resourceManager->GetMaterial("char").lock());
	auto fbxMtl2 = std::dynamic_pointer_cast<PhongMaterial>(resourceManager->GetMaterial("mistress").lock());
	fbxMtl1->ambient = fbxMtl2->ambient = material.ambient;
	fbxMtl1->diffuse = fbxMtl2->diffuse = material.diffuse;
	fbxMtl1->Create();
	fbxMtl2->Create();
	OutputDebugStringA("読み込みOK\n");
	loadFlag = true;
}
