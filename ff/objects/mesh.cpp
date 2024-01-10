#include "mesh.h"

namespace ff
{
	Mesh::Ptr Mesh::create(const Geometry::Ptr& geometry, const Material::Ptr& material)
	{
		return std::make_shared<Mesh>(geometry, material);
	}

	Mesh::Mesh(const Geometry::Ptr& geometry, const Material::Ptr& material) noexcept
		: RenderableObject(geometry, material)
	{
		mIsMesh = true;
	}

	Mesh::~Mesh() noexcept = default;
}
