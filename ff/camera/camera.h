#pragma once
#include "../global/base.h"
#include "../core/object3D.h"

namespace ff {

	/// worldMatrix:将顶点从模型坐标系，转换到世界坐标系
	/// viewMatrix：将顶点从世界坐标系，转换到模型坐标系
	/// 逆矩阵（Inverse Matrix）A与B互为逆矩阵，那么A * B = I（单位阵）
	/// Camera:一个是作为游戏主视角，作为光照的主视角
	class Camera:public Object3D {
	public:
		using Ptr = std::shared_ptr<Camera>;
	
		Camera() noexcept;

		~Camera() noexcept;

		auto updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept -> glm::mat4 override;

		/// \brief 获得摄像机的世界坐标矩阵的逆矩阵
		/// \return 
		auto getWorldMatrixInverse() const noexcept -> glm::mat4 { return mWorldMatrixInverse; }

		/// \brief 获得摄像机的投影矩阵
		/// \return 
		auto getProjectionMatrix() const noexcept -> glm::mat4 { return mProjectionMatrix; }

		/// \brief 更新摄像机的投影矩阵
		/// \return 
		virtual auto updateProjectionMatrix() noexcept -> glm::mat4 = 0;

	protected:
		glm::mat4 mWorldMatrixInverse = glm::mat4(1.0f);
		glm::mat4 mProjectionMatrix = glm::mat4(1.0f);

	};
}