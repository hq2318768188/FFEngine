#pragma once
#include "../global/base.h"
#include "../camera/camera.h"
#include "../core/object3D.h"
#include "../objects/mesh.h"
#include "../scene/scene.h"
#include "renderTarget.h"
#include "driver/driverAttributes.h"
#include "driver/driverBindingState.h"
#include "driver/driverPrograms.h"
#include "driver/driverWindow.h"
#include "driver/driverRenderList.h"
#include "driver/driverTextures.h"
#include "driver/driverObjects.h"
#include "driver/driverInfo.h"
#include "driver/driverState.h"
#include "driver/driverMaterials.h"
#include "driver/driverBackground.h"
#include "driver/driverRenderState.h"
#include "driver/driverRenderTargets.h"
#include "driver/driverShadowMap.h"
#include "../math/frustum.h"

namespace ff
{
	class Renderer : public std::enable_shared_from_this<Renderer>
	{
	public:
		friend class DriverShadowMap;

		struct Descriptor
		{
			uint32_t mWidth{800};
			uint32_t mHeight{600};

			/// TODO 是否抗锯齿........
		};

		using OnSizeCallback = std::function<void(int width, int height)>;

		using Ptr = std::shared_ptr<Renderer>;
		static Ptr create(const Descriptor& descriptor);

		Renderer(const Descriptor& descriptor) noexcept;

		~Renderer() noexcept;

		/// \brief 渲染刷新
		/// \param scene 场景
		/// \param camera 摄像机
		/// \return 
		auto render(Scene::Ptr scene, Camera::Ptr camera) -> bool;

		/// \brief 双缓冲与垂直同步
		auto swap() const noexcept -> void;

		auto setSize(int width, int height) noexcept -> void;

		auto setRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept -> void;

		auto setFrameSizeCallBack(const OnSizeCallback& callback) noexcept -> void;

		auto setMouseMoveCallBack(const DriverWindow::MouseMoveCallback& callback) noexcept -> void;

		auto setMouseActionCallback(const DriverWindow::MouseActionCallback& callback) noexcept -> void;

		auto setKeyboardActionCallBack(const DriverWindow::KeyboardActionCallback& callback) noexcept -> void;

		RenderTarget::Ptr getRenderTarget() const noexcept;

		/// \brief 设置刷新背景色
		/// \param r 
		/// \param g 
		/// \param b 
		/// \param a 
		auto setClearColor(float r, float g, float b, float a) noexcept -> void;

		/// \brief 获得当前的刷新背景色
		/// \return 
		auto getClearColor() const noexcept -> glm::vec4;

		void enableShadow(bool enable) noexcept;

		/// \brief 清除 colorbuffer
		/// \param color 
		/// \param depth 
		/// \param stencil 
		auto clear(bool color = true, bool depth = true, bool stencil = true) const noexcept -> void;

	public:
		/// 每一次绘制，是否需要自动擦除ColorBuffer
		bool mAutoClear{true};

	private:
		/// ///////////////////////////// 层级渲染 /////////////////////////////////////// /// 

		/// \brief				project：将层级架构的组织，展开成为列表
		/// \param object		当前需要被project的object，
		/// \param groupOrder	groupOrder 当前其所处于的group的渲染优先级
		/// \param sortObjects	sortObjects 是否在渲染列表中，对item进行排序
		auto projectObject(const Object3D::Ptr& object, uint32_t groupOrder, bool sortObjects) noexcept -> void;


		/// \brief
		/// 第一层级，在场景级别，进行一些状态的处理与设置，并且根据
		/// 实体/透明物体进行队列渲染-renderObjects
		/// \param currentRenderList 
		/// \param scene 
		/// \param camera 
		auto renderScene(const DriverRenderList::Ptr& currentRenderList, const Scene::Ptr& scene,
		                 const Camera::Ptr& camera) noexcept -> void;

		/// \brief
		///	第二层级，在队列级别，进行一些状态的处理与设置
		/// 依次调用每个渲染单元，进入到renderObject
		/// \param renderItems 
		/// \param scene 
		/// \param camera 
		auto renderObjects(
			const std::vector<RenderItem::Ptr>& renderItems,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera) noexcept -> void;


		/// \brief
		///	第三层级，在单个渲染单元层面上，进行一些状态的处理与设置
		/// 并且调用跟API相关深重的renderBufferDirect
		/// \param object 
		/// \param scene 
		/// \param camera 
		/// \param geometry 
		/// \param material 
		auto renderObject(
			const RenderableObject::Ptr& object,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material) noexcept -> void;

		/// \brief 在单个渲染单元层面上
		///	第四层级，最底层的所有的跟OpenGL的状态机最密集的一层
		/// \param object 
		/// \param scene 
		/// \param camera 
		/// \param geometry 
		/// \param material 
		auto renderBufferDirect(
			const RenderableObject::Ptr& object,
			const Scene::Ptr& scene,
			const Camera::Ptr& camera,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material) noexcept -> void;

		/// \brief 设置本Material的Program
		/// \param camera 
		/// \param scene 
		/// \param geometry 
		/// \param material 
		/// \param object 
		/// \return 
		auto setProgram(
			const Camera::Ptr& camera,
			const Scene::Ptr& scene,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material,
			const RenderableObject::Ptr& object) noexcept -> DriverProgram::Ptr;

		/// \brief 得到与本Material对应的Program
		/// \param material 
		/// \param scene 
		/// \param object 
		/// \return 
		auto getProgram(
			const Material::Ptr& material,
			const Scene::Ptr& scene,
			const RenderableObject::Ptr& object) noexcept -> DriverProgram::Ptr;

		/// \brief	更新了本Material跟其对应的DriverMaterial的关键变量
		/// \param material 
		/// \param parameters 
		auto updateCommonMaterialProperties(
			const Material::Ptr& material,
			const DriverProgram::Parameters::Ptr& parameters) noexcept -> void;

		/// \brief 当前Material是否需要光照
		/// \param material 
		/// \return 
		auto materialNeedsLights(const Material::Ptr& material) noexcept -> bool;

		void makeLightsNeedUpdate(UniformHandleMap& lightsUniformMap) noexcept;

	private:
		int mWidth{800};
		int mHeight{600};

		OnSizeCallback mOnSizeCallback{nullptr};

		bool mSortObject{true};

		glm::mat4 mCurrentViewMatrix = glm::mat4(1.0f);

		glm::vec4 mViewport{};

		RenderTarget::Ptr mCurrentRenderTarget{nullptr};

		DriverWindow::Ptr mWindow{nullptr};
		DriverRenderList::Ptr mRenderList{nullptr};
		/// \brief 纹理
		DriverTextures::Ptr mTextures{nullptr};
		DriverAttributes::Ptr mAttributes{nullptr};
		DriverInfo::Ptr mInfos{nullptr};
		DriverState::Ptr mState{nullptr};
		DriverObjects::Ptr mObjects{nullptr};
		DriverGeometries::Ptr mGeometries{nullptr};
		DriverPrograms::Ptr mPrograms{nullptr};
		DriverBindingStates::Ptr mBindingStates{nullptr};
		DriverMaterials::Ptr mMaterials{nullptr};
		DriverBackground::Ptr mBackground{nullptr};
		DriverRenderState::Ptr mRenderState{nullptr};
		DriverRenderTargets::Ptr mRenderTargets{nullptr};
		DriverShadowMap::Ptr mShadowMap{nullptr};

		Frustum::Ptr mFrustum{nullptr};

		/// dummy objects
		Scene::Ptr mDummyScene = Scene::create();
	};
}
