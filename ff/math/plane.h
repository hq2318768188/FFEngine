#pragma once

#include "../global/base.h"

namespace ff {

	class Plane {
	public:
		using Ptr = std::shared_ptr<Plane>;
		static Ptr create(const glm::vec3& normal, float constant) {
			return std::make_shared<Plane>(normal, constant);
		}

		Plane(const glm::vec3& normal, float constant) noexcept {
			mNormal = glm::normalize(normal);
			mConstant = constant;
		}

		~Plane() noexcept {}

		
		/// \brief 求取一个点到平面的距离（与法线通向者距离为正，与法线反向者距离为负）
		/// \param point 
		/// \return 
		auto distanceToPoint(const glm::vec3& point) const noexcept -> float
		{
			return glm::dot(mNormal, point) + mConstant;
		}

		/// \brief 设置平面的矩阵
		/// \param x 
		/// \param y 
		/// \param z 
		/// \param w 
		auto setComponents(float x, float y, float z, float w) noexcept -> void
		{
			mNormal.x = x;
			mNormal.y = y;
			mNormal.z = z;
			
			auto length = glm::length(mNormal);
			mNormal /= length;
			mConstant = w/length;
		}

	public:
		glm::vec3	mNormal = glm::vec3(0.0f);
		float		mConstant{ 0.0f };
	};
}