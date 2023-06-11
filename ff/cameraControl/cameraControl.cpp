#include "cameraControl.h"

namespace ff {

	CameraControl::CameraControl(const Camera::Ptr& camera) noexcept {
		mCamera = camera;
	}

	CameraControl::~CameraControl() noexcept {}

	auto CameraControl::onKeyboard(const KeyBoardState& action) noexcept -> void {}

	auto CameraControl::onMouseAction(const MouseAction& action) noexcept -> void {}

	auto CameraControl::onMouseMove(double xpos, double ypos) noexcept -> void {}
}