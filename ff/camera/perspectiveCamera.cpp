#include "perspectiveCamera.h"

namespace ff {

	PerspectiveCamera::PerspectiveCamera(float near, float far, float aspect, float fov) noexcept {
		mIsPerpectiveCamera = true;
		mNear = near;
		mFar = far;
		mAspect = aspect;
		mFov = fov;

		updateProjectionMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera() noexcept {}

	auto PerspectiveCamera::updateProjectionMatrix() noexcept -> glm::mat4
	{
		mProjectionMatrix = glm::perspective(glm::radians(mFov), mAspect, mNear, mFar);

		return mProjectionMatrix;
	}
}