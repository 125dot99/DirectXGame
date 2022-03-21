#pragma once
#include <memory>

namespace gamelib
{
//テンプレート生成クラス
class Factory
{
private:
	Factory() = delete;
	~Factory() = delete;
public:
	template<class T, class... Args>
	static inline T* Create(Args&&... args)
	{
		return new T(args...);
	}

	//unique_ptrで生成
	template<class T, class... Args>
	static inline std::unique_ptr<T> CreateUnique(Args&&... args)
	{
		return std::make_unique<T>(args...);
	}

	//shared_ptrで生成
	template<class T, class... Args>
	static inline std::shared_ptr<T> CreateShared(Args&&... args)
	{
		return std::make_shared<T>(args...);
	}
};
} // namespace gamelib

