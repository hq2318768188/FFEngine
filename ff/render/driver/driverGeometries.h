#pragma once
#include "../../global/base.h"
#include "../../core/geometry.h"
#include "driverAttributes.h"
#include "driverInfo.h"
#include "driverBindingState.h"

namespace ff {

	/// 在几何数据层面上，对backeng的数据进行管理
	class DriverGeometries {
	public:
		using Ptr = std::shared_ptr<DriverGeometries>;
		static Ptr create(
			const DriverAttributes::Ptr& attributes,
			const DriverInfo::Ptr& info,
			const DriverBindingStates::Ptr& bindingStates
		) {
			return std::make_shared <DriverGeometries>(attributes, info, bindingStates);
		}

		DriverGeometries(
			const DriverAttributes::Ptr& attributes,
			const DriverInfo::Ptr& info,
			const DriverBindingStates::Ptr& bindingStates
		) noexcept;

		~DriverGeometries() noexcept;

		auto get(const Geometry::Ptr& geometry) noexcept -> Geometry::Ptr;

		auto onGeometryDispose(const EventBase::Ptr& event) -> void;

		auto update(const Geometry::Ptr& geometry) const noexcept -> void;
		
	private:
		DriverAttributes::Ptr mAttributes{ nullptr };
		DriverInfo::Ptr mInfo{ nullptr };

		/// 顶点绑定VAO有关系
		DriverBindingStates::Ptr mBindingStates{ nullptr };

		std::unordered_map<ID, bool> mGeometries{};
	};
}