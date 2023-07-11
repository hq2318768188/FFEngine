#pragma once
#include "../../global/base.h"
#include "../../material/material.h"
#include "../renderTarget.h"
#include "driverRenderTargets.h"

namespace ff {

	/// 配置渲染管线
	/// 1 深度检测配置
	/// 2 颜色混合配置
	/// 3 光栅化配置
	/// 4 其他相关配置
	class DriverState {
	public:
		struct RasterState {
			Side		mSide{ Side::None };
			FrontFace	mFrontFace{ FrontFace::None };
		};

		struct ColorState {
			glm::vec4 mClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		};

		struct BlendingState {
			BlendingType		mBlendingType{ BlendingType::None };
			bool				mTransparent{ false };
			BlendingFactor		mBlendSrc{ BlendingFactor::None };
			BlendingFactor		mBlendDst{ BlendingFactor::None };
			BlendingEquation	mBlendEquation{ BlendingEquation::None };
			BlendingFactor		mBlendSrcAlpha{ BlendingFactor::None };
			BlendingFactor		mBlendDstAlpha{ BlendingFactor::None };
			BlendingEquation	mBlendEquationAlpha{ BlendingEquation::None };
		};

		struct DepthState {
			bool			mDepthTest{ false };
			bool			mDepthWrite{ false };
			CompareFunction mDepthFunction{ CompareFunction::None };
			double			mDepthClearColor{ -1.0f };
		};

		using Ptr = std::shared_ptr<DriverState>;
		static Ptr create() { return std::make_shared<DriverState>(); }

		DriverState() noexcept;

		~DriverState() noexcept;

		auto viewport(const glm::vec4& viewport) noexcept -> void;

		/// \brief 检查上一个物体渲染使用的Program是否跟当前的一样, 不一样刷新
		/// \param program 
		/// \return 
		auto useProgram(GLuint program) noexcept -> bool;

		/// \brief 设置材质
		/// \param material 
		auto setMaterial(const Material::Ptr& material) noexcept -> void;

		auto bindFrameBuffer(const GLuint& frameBuffer) noexcept -> void;

		auto setClearColor(float r, float g, float b, float a) noexcept -> void;

		auto setBlending(
			BlendingType blendingType,
			bool transparent = false,
			BlendingFactor blendSrc = BlendingFactor::SrcAlpha,
			BlendingFactor blendDst = BlendingFactor::OneMinusSrcAlpha,
			BlendingEquation blendEquation = BlendingEquation::AddEquation,
			BlendingFactor blendSrcAlpha = BlendingFactor::SrcAlpha,
			BlendingFactor blendDstAlpha = BlendingFactor::OneMinusSrcAlpha,
			BlendingEquation blendEquationAlpha = BlendingEquation::AddEquation
			) noexcept -> void;
		
		auto setDepth(
			bool depthTest,
			bool depthWrite,
			CompareFunction depthFunction,
			double depthClearColor
			) noexcept -> void;

		auto getClearColor() const noexcept -> glm::vec4;

	private:
		auto setBlendingInternal(
			bool transparent,
			BlendingFactor blendSrc,
			BlendingFactor blendDst,
			BlendingEquation blendEquation,
			BlendingFactor blendSrcAlpha,
			BlendingFactor blendDstAlpha,
			BlendingEquation blendEquationAlpha
			) noexcept -> void;

	private:
		RasterState		mCurrentRaster;
		BlendingState	mCurrentBlending;
		DepthState		mCurrentDepth;
		ColorState		mCurrentColor;

		GLuint	mCurrentProgram{ 0 };
		glm::vec4 mCurrentViewport{};
		GLuint	mCurrentFrameBuffer{ 0 };
	};
}