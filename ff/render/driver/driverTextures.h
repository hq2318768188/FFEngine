#pragma once
#include "../../global/base.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"
#include "../../global/eventDispatcher.h"
#include "driverInfo.h"
#include "../renderTarget.h"
#include "driverRenderTargets.h"

namespace ff {

	class DriverTexture {
	public:
		using Ptr = std::shared_ptr<DriverTexture>;
		static Ptr create() {
			return std::make_shared <DriverTexture>();
		}

		DriverTexture() noexcept;

		~DriverTexture() noexcept;
	public:
		 
		/// \brief 解除纹理id 
		auto dispose() noexcept -> void;
	public:
		/// \brief 通过glGenTextures获得的texture的编号
		GLuint	mHandle{ 0 };

	};
	
	/*
	* 对于每一个texture，要确定在正式使用的时候，才能进行解析
	* 如何判定一个texture正式使用了呢？
	* 本texture被作为usniform进行upload的时候，才是其正式使用的契机
	*/
	class DriverTextures {
	public:
		using Ptr = std::shared_ptr<DriverTextures>;
		static Ptr create(const DriverInfo::Ptr& info, const DriverRenderTargets::Ptr& renderTargets) {
			return std::make_shared <DriverTextures>(info, renderTargets);
		}

		DriverTextures(const DriverInfo::Ptr& info, const DriverRenderTargets::Ptr& renderTargets) noexcept;

		~DriverTextures() noexcept;
	public:
		/// \brief  传入texture，获得其对应的DriverTexture
		/// \param texture 
		/// \return  
		auto get(const Texture::Ptr& texture) noexcept -> DriverTexture::Ptr;

		/// \brief 将texture对应的Driver Texture当中的mHandle，绑定到textureUnit的slot上
		///        GL_TEXTURE0 GL_TEXTURE1....
		/// \param texture 
		/// \param textureUnit 
		auto bindTexture(const Texture::Ptr& texture, GLenum textureUnit) -> void;

		auto setupRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept -> void;

		auto onTextureDestroy(const EventBase::Ptr& e) noexcept -> void;

	private:
		/// \brief 要么新建一个texture ， 要么更新原有texture的属性数据或者内容数据
		/// \param texture 
		auto update(const Texture::Ptr& texture) noexcept -> void;

		/// \brief 安装一个DriverTexture (opengl底层创建纹理)
		/// \param texture 
		/// \return  
		auto setupDriverTexture(const Texture::Ptr& texture) noexcept -> DriverTexture::Ptr;

		auto setupFBOColorAttachment(const GLuint& fbo, const GLenum& target,
		                             const Texture::Ptr& texture) noexcept -> void;

		void setupFBODepthStencilAttachment(const RenderTarget::Ptr& renderTarget) noexcept;

		void setupDepthTexture(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) noexcept;

		void setupDepthRenderBuffer(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget);

	private:
		DriverInfo::Ptr								mInfo{ nullptr };
		DriverRenderTargets::Ptr					mRenderTargets{ nullptr };
		std::unordered_map<ID, DriverTexture::Ptr>	mTextures{};
	};
}