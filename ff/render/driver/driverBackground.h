#pragma once
#include "../../global/base.h"
#include "../../global/constant.h"
#include "driverObjects.h"
#include "driverRenderList.h"
#include "../../scene/scene.h"
#include "../../objects/mesh.h"

namespace ff
{
	/*
	* 1 准备绘制天空盒的数据
	* 2 对渲染画布进行清理，对当前使用的RenderTarget的ColorBuffer进行清理
	*/
	class Renderer;

	class DriverBackground
	{
	public:
		using Ptr = std::shared_ptr<DriverBackground>;
		static Ptr create(Renderer* renderer, const DriverObjects::Ptr& objects);

		DriverBackground(Renderer* renderer, const DriverObjects::Ptr& objects) noexcept;

		~DriverBackground() noexcept;

		/// \brief  渲染背景
		/// \param renderList 
		/// \param scene 
		auto render(const DriverRenderList::Ptr& renderList, const Scene::Ptr& scene) -> void;

	private:
		Renderer* mRenderer{nullptr};
		DriverObjects::Ptr mObjects{nullptr};

		/// 用于绘制天空盒的BoxMesh
		Mesh::Ptr mBoxMesh{nullptr};
	};
}
