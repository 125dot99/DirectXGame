#pragma once
namespace gamelib
{
class ISceneManager
{
public:
	virtual ~ISceneManager() = default;

	/// <summary>
	/// ポインタ付け替え
	/// </summary>
	/// <param name="newScene">次のシーンのポインタ</param>
	virtual void ChangeScene(class BaseScene* newScene) = 0;
};
} // namespace gamelib