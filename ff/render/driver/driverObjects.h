#pragma once
#include "../../global/base.h"
#include "../../objects/renderableObject.h"
#include "driverGeometries.h"
#include "driverAttributes.h"
#include "driverInfo.h"

namespace ff {
	class DriverObjects {
	public:
		using Ptr = std::shared_ptr<DriverObjects>;
		static Ptr create(
			const DriverGeometries::Ptr& geometries,
			const DriverAttributes::Ptr& attributes,
			const DriverInfo::Ptr& info
		) {
			return std::make_shared <DriverObjects>(geometries, attributes, info);
		}

		DriverObjects(
			const DriverGeometries::Ptr& geometries,
			const DriverAttributes::Ptr& attributes,
			const DriverInfo::Ptr& info
		) noexcept;

		~DriverObjects() noexcept;

		auto update(const RenderableObject::Ptr& object) noexcept -> Geometry::Ptr;

	private:
		/// TODO  还差一个instance绘制的功能

		std::unordered_map<ID, uint32_t> mUpdateMap{};

		DriverInfo::Ptr mInfo{ nullptr };
		DriverAttributes::Ptr mAttributes{ nullptr };
		DriverGeometries::Ptr mGeometries{ nullptr };
	};
}