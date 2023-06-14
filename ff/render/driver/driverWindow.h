#pragma once

#include "../../global/base.h"
#include "../../global/constant.h"

namespace ff {

	class Renderer;
	/// 使用的库是glfw
	class DriverWindow {
	public:

		/// 窗体发生size的变动
		/// 窗体变动比较特殊，外界主函数当中的callback函数，注册在了renderer里面，在renderer.cpp
		/// 当中，有一个onFrameSizeCallback被注册到了DriverWindow，当事件发生的时候，
		/// glfw会首先调用frameSizeCallback,在其中，调用了onFrameSizeCallback
		/// 在onFrameSizeCallback当中，调用了renderer->setSize(xxx)
		/// setSize当中,对renderer进行了状态调整,随后调用外界的callback函数
		using FrameSizeCallback = std::function<void(DriverWindow* pDriverWindow, int width, int height)>;

		/// 鼠标移动回调
		using MouseMoveCallback = std::function<void(double xpos, double ypos)>;

		/// 键盘响应回调 
		using KeyboardActionCallback = std::function<void(KeyBoardState)>;

		/// 鼠标事件响应
		using MouseActionCallback = std::function<void(MouseAction)>;

		/// 智能指针管理
		using Ptr = std::shared_ptr<DriverWindow>;
		static Ptr create(Renderer* renderer, const int& width, const int& height) { 
			return std::make_shared<DriverWindow>(renderer, width, height);
		}

		DriverWindow(Renderer* renderer, const int &width, const int &height) noexcept;

		~DriverWindow() noexcept;

		auto getWindow() const  noexcept { return mWindow; }

		/// \brief 每一帧渲染前，都会调用processEvent，对事件进行响应
		/// \return 
		auto processEvent() noexcept -> bool;

		/// \brief 双缓冲的更换
		auto swap() const noexcept -> void;

		/// ////////////////////////////////////////// 事件回调函数 ////////////////////////////////// ///

		/// \brief 设置窗口大小变化事件的回调函数
		/// \param callback 
		auto setFrameSizeCallBack(const FrameSizeCallback& callback) noexcept -> void;

		/// \brief 设置鼠标移动事件的回调函数
		/// \param callback 
		auto setMouseMoveCallBack(const MouseMoveCallback& callback) noexcept -> void;

		/// \brief 设置鼠标按下事件的回调函数
		/// \param callback 
		auto setMouseActionCallback(const MouseActionCallback& callback) noexcept -> void;

		/// \brief 设置键盘按键事件的回调函数
		/// \param callback 
		auto setKeyboardActionCallBack(const KeyboardActionCallback& callback) noexcept -> void;

	private:
		/// //// glfw相关事件，会先回调下方函数，如果外界已经给到window注册的接口，那么在下方函数中会调用外界回调函数 /// ///

		static auto frameSizeCallback(GLFWwindow* window, int width, int height) noexcept -> void;

		static auto mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) noexcept -> void;

		static auto mouseActionCallback(GLFWwindow* window, int button, int action, int mods) noexcept -> void;

	public:
		Renderer* mRenderer = nullptr;

	private:
		int mWidth{ 0 };
		int mHeight{ 0 };

		KeyBoardState mKeyState{};

		/// 允许外界设置相关回调函数
		FrameSizeCallback		mFrameSizeCallback{ nullptr };
		MouseMoveCallback		mMouseMoveCallback{ nullptr };
		KeyboardActionCallback	mKeyboardActionCallback{ nullptr };
		MouseActionCallback		mMouseActionCallback{ nullptr };

		GLFWwindow* mWindow{ NULL };
	};
}
