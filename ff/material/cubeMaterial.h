#pragma once
#include "material.h"

namespace ff
{
	class CubeMaterial : public Material
	{
	public:
		using Ptr = std::shared_ptr<CubeMaterial>;
		static Ptr create();

		CubeMaterial() noexcept;

		~CubeMaterial() noexcept;
	};
}
