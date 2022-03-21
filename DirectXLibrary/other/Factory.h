#pragma once
#include <memory>

namespace gamelib
{
//�e���v���[�g�����N���X
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

	//unique_ptr�Ő���
	template<class T, class... Args>
	static inline std::unique_ptr<T> CreateUnique(Args&&... args)
	{
		return std::make_unique<T>(args...);
	}

	//shared_ptr�Ő���
	template<class T, class... Args>
	static inline std::shared_ptr<T> CreateShared(Args&&... args)
	{
		return std::make_shared<T>(args...);
	}
};
} // namespace gamelib

