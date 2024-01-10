#pragma once
#include "loader.h"
#include "../textures/cubeTexture.h"

namespace ff
{
	class CubeTextureLoader : public Loader
	{
	public:
		CubeTextureLoader() noexcept;

		~CubeTextureLoader() noexcept;

		/// \brief 导入图片
		/// \param paths 图片绝对路径
		/// \return  
		static auto load(const std::vector<std::string>& paths) noexcept -> CubeTexture::Ptr;
	};
}
