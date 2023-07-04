#pragma once
#include "../../global/base.h"
#include "../../objects/renderableObject.h"
#include "../../material/material.h"
#include "driverUniforms.h"
#include "driverLights.h"
#include "driverShadowMap.h"
#include "../shaders/uniformsLib.h"

namespace ff {

	class DriverPrograms;
	class DriverProgram {
		friend DriverPrograms;
	public:

		struct Parameters {
			using Ptr = std::shared_ptr<Parameters>;
			static Ptr create() { return std::make_shared<Parameters>(); }

			std::string		mShaderID;							/// material 的Typename
			std::string		mVertex;							/// vs的代码
			std::string		mFragment;							/// fs的代码

			bool			mInstancing{ false };				/// 是否启用实例绘制,TODO
			bool			mHasNormal{ false };				/// 本次绘制的模型是否有法线
			bool			mHasUV{ false };					/// 本次绘制的模型是否有uv
			bool			mHasColor{ false };					/// 本次绘制的模型是否有顶点颜色
			bool			mHasDiffuseMap{ false };			/// 本次绘制的模型所使用的材质是否有diffuseMap
			bool			mHasEnvCubeMap{ false };			/// 本次绘制的模型所使用的材质是否有环境贴图
			bool			mHasSpecularMap{ false };			/// 本次绘制的模型所使用的材质是否有镜面反射贴图

			bool			mShadowMapEnabled{ false };			/// 是否启用阴影
			uint32_t		mDirectionalLightCount{ 0 };
			uint32_t		mNumDirectionalLightShadows{ 0 };

			bool			mUseTangent{ false };
			bool			mUseNormalMap{ false };

			bool			mSkinning{ false };
			uint32_t		mMaxBones{ 0 };

			uint32_t		mDepthPacking{ 0 };
		};

		using Ptr = std::shared_ptr<DriverProgram>;
		static Ptr create(const Parameters::Ptr& parameters) {
			return std::make_shared <DriverProgram>(parameters);
		}

		DriverProgram(const Parameters::Ptr& parameters) noexcept;

		~DriverProgram() noexcept;

		auto getID() const noexcept { return mID; }

		auto getCacheKey() const noexcept { return mCacheKey; }

		GLuint		mProgram{ 0 };

		auto uploadUniforms(UniformHandleMap& uniformGroup, const DriverTextures::Ptr& textures) -> void;

	private:
		auto replaceAttributeLocations(std::string& shader) const noexcept -> void;
		auto replaceLightNumbers(std::string& shader, const Parameters::Ptr& parameters) const noexcept -> void;

		auto getExtensionString() noexcept -> std::string;

	private:
		uint32_t	mID{ 0 };			/// driverProgram 自己的id号
		HashType	mCacheKey{ 0 };		/// 由parameters参数合集计算出来的hash值
		uint32_t	mRefCount{ 0 };		/// 控制外界有多少引用本Program的renderItem
		DriverUniforms::Ptr mUniforms = nullptr;
	};

	/// 1 对于DriverProgram的管理,存储成了一个map，key是program的哈希值，value就是DriverProgram的智能指针
	/// 2 对外暴露辅助逻辑进行的接口
	class DriverPrograms {
	public:
		using Ptr = std::shared_ptr<DriverPrograms>;
		static Ptr create() {
			return std::make_shared <DriverPrograms>();
		}

		DriverPrograms() noexcept;

		~DriverPrograms() noexcept;

		auto acquireProgram(const DriverProgram::Parameters::Ptr& parameters,
		                    HashType cacheKey) noexcept -> DriverProgram::Ptr;

		/// \brief				根据传入Material类型的不同，返回其必须的UniformHandleMap
		/// \param material 
		/// \return 
		auto getUniforms(const Material::Ptr& material) noexcept -> UniformHandleMap;

		
		/// \brief				提取创建shader所必要的信息，组成一个parameters返回
		/// \param material		当前渲染物体的材质
		/// \param object		当前渲染物体的object3D
		/// \param lights		当前渲染物体的光源信息
		/// \param shadowMap	当前渲染物体的阴影信息
		/// \return 
		auto getParameters(
			const Material::Ptr& material,
			const Object3D::Ptr& object,
			const DriverLights::Ptr& lights,
			const DriverShadowMap::Ptr& shadowMap) const noexcept -> DriverProgram::Parameters::Ptr;

		/// \brief				将parameters做成字符串，然后进行哈希运算，得到最终的哈希结果
		/// \param parameters 
		/// \return 
		auto getProgramCacheKey(const DriverProgram::Parameters::Ptr& parameters) noexcept -> HashType;

		/// \brief				外界某个renderItem释放了对本Driverprogram的使用
		/// \param program 
		auto release(const DriverProgram::Ptr& program) noexcept -> void;

	private:
		/// key-paramters做成的哈希值，value-用本parameters生成的driverProgram
		std::unordered_map<HashType, DriverProgram::Ptr> mPrograms{};
	};
}