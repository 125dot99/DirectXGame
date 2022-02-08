#pragma once
#include "ISceneManager.h"
namespace gamelib
{
//シーン抽象クラス
class BaseScene abstract
{
protected:
	/// <summary>
	/// シーンを切り替えるか
	/// </summary>
	bool nextSceneState;
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() = 0;
	
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;
	
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;
	
	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	/// <param name="sceneManager">シーンマネージャーのポインタ</param>
	virtual void NextScene(ISceneManager* pSceneManager) = 0;
	
	/// <summary>
	/// シーンを切り替える状態を返す
	/// </summary>
	/// <returns>シーンを切り替えるか</returns>
	inline bool GetNextScene() const { return nextSceneState; }
};

//シーンの子クラス
class BaseChildScene
{
public:
	BaseChildScene() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseChildScene() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() {}

	/// <summary>
	/// シーンの状態を返す
	/// </summary>
	virtual bool IsState(const char* typeName) const { return "None" == typeName; }

	/// <summary>
	/// 次のシーンに切り替わるか
	/// </summary>
	virtual bool IsNext() const { return false; }
};
} // namespace scene