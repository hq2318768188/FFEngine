#pragma once
#include <mutex>

#include "../global/base.h"
#include "../global/constant.h"
#include "../textures/source.h"
#include "../global/eventDispatcher.h"

namespace ff
{
	/// 从中读取到已经保存过的source
	/// 将新的source 缓存到这个类里面
	/// source在被其他对象放弃使用权的时候，能够知晓，并且没有人用的时候，析构
	class Cache
	{
	public:
		static Cache* getInstance();

		Cache() noexcept;

		~Cache() noexcept;

		/// \brief 从缓存中获得source
		/// \param path 
		/// \return 
		auto getSource(const std::string& path) noexcept -> Source::Ptr;

		/// \brief 缓存source
		/// \param path 
		/// \param source 
		auto cacheSource(const std::string& path, Source::Ptr source) noexcept -> void;

		/// \brief 监听
		/// \param e 
		auto onSourceRelease(const EventBase::Ptr& e) -> void;

	private:
		static Cache* mInstance;

		/// hashType = size_t
		std::unordered_map<HashType, Source::Ptr> mSources{};

		std::mutex mMutex;
	};
}
