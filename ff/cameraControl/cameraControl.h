#pragma once
#include "../global/base.h"
#include "../global/constant.h"
#include "../camera/camera.h"

namespace ff {

	/// 任务：根据键盘鼠标，更改camera的矩阵
	/// 种类：gameCameraControl wasd 鼠标右键 trackBallControl cad/cae软件当中
	class CameraControl {
	public:
		using Ptr = std::shared_ptr<CameraControl>;

		CameraControl(const Camera::Ptr& camera) noexcept;

		~CameraControl() noexcept;

		
		/// \brief 键盘事件
		///	本质上来讲就是bit位集合，一共108键
		/// \param action 
		virtual auto onKeyboard(const KeyBoardState& action) noexcept -> void;

		/// \brief 鼠标按下事件
		/// \param action 
		virtual auto onMouseAction(const MouseAction& action) noexcept -> void;

		/// \brief 鼠标移动事件
		/// \param xpos 
		/// \param ypos 
		virtual auto onMouseMove(double xpos, double ypos) noexcept -> void;

	protected:
		Camera::Ptr	mCamera{ nullptr };/// 正交相机  透视相机
	};
}