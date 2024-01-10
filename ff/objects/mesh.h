#pragma once
#include "../global/base.h"
#include "renderableObject.h"

namespace ff
{
	class Mesh : public RenderableObject
	{
	public:
		using Ptr = std::shared_ptr<Mesh>;
		static Ptr create(const Geometry::Ptr& geometry, const Material::Ptr& material);

		Mesh(const Geometry::Ptr& geometry, const Material::Ptr& material) noexcept;

		~Mesh() noexcept override;
	};
}
