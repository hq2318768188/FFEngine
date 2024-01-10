#include "cubeMaterial.h"

namespace ff
{
	CubeMaterial::Ptr CubeMaterial::create()
	{
		return std::make_shared<CubeMaterial>();
	}

	CubeMaterial::CubeMaterial() noexcept
	{
		mType = MaterialName::CubeMaterial;
		mIsCubeMaterial = true;
		mDrawMode = DrawMode::Triangles;
	}

	CubeMaterial::~CubeMaterial() noexcept = default;
}
