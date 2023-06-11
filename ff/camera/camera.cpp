#include "camera.h"

namespace ff {

	Camera::Camera() noexcept {
		mIsCamera = true;
	}

	Camera::~Camera() noexcept {}

	auto Camera::updateWorldMatrix(bool updateParent, bool updateChildren) noexcept -> glm::mat4
	{
		Object3D::updateWorldMatrix(updateParent, updateChildren);
		mWorldMatrixInverse = glm::inverse(mWorldMatrix);

		return mWorldMatrix;
	}
}