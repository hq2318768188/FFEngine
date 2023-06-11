#pragma once 
#include "cameraControl.h"
#include "../global/constant.h"

namespace ff {

	class GameCameraControl :public CameraControl {
	public:
		static constexpr int MoveFront = 0;
		static constexpr int MoveBack = 1;
		static constexpr int MoveLeft = 2;
		static constexpr int MoveRight = 3;

		using Ptr = std::shared_ptr<GameCameraControl>;
		static Ptr create(const Camera::Ptr& camera) {
			return std::make_shared<GameCameraControl>(camera);
		}

		GameCameraControl(const Camera::Ptr& camera) noexcept;

		~GameCameraControl() noexcept;

		/// \brief 设置鼠标控制摄像机的速度
		/// \param speed 
		auto setSpeed(float speed) noexcept -> void;

		/// \brief 设置鼠标控制摄像机的灵敏度
		/// \param s 
		auto setSensitivity(float s) noexcept -> void;

		/// \brief 摄像机键盘事件
		/// \param action 
		auto onKeyboard(const KeyBoardState& action) noexcept -> void override;

		/// \brief 摄像机鼠标按下事件
		/// \param action 
		auto onMouseAction(const MouseAction& action) noexcept -> void override;

		/// \brief 摄像机鼠标移动事件
		/// \param xpos 
		/// \param ypos 
		auto onMouseMove(double xpos, double ypos) noexcept -> void override;

		/// \brief 刷新摄像机
		auto update() noexcept -> void;

	private:
		float		mSpeed{ 0.1f };						/// 移动速度
		float		mSensitivity{ 0.1f };				/// 灵敏度，鼠标移动造成多大程度的旋转

		glm::vec3	mFront = glm::vec3(0.0f);		/// 当前相机的冲向
		float		mPitchAngle{ 0.0f };
		float		mYawAngle{ -90.0f };

		/// control parameters
		glm::vec2	mCurrentMousePosition = glm::vec2(0.0f);
		/// 用来记录当前正在移动的方向，可以同时多个
		glm::bvec4	mMoveState = glm::bvec4(false);

		/// 是否正在旋转,鼠标右键点住之后，可以拖动上下左右旋转视角,抬起之后，就不能了
		bool		mRotationState = false;
		bool		mMouseStateReset = true;
	};
}