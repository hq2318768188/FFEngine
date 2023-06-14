#include "renderableObject.h"

namespace ff {

	RenderableObject::RenderableObject(const Geometry::Ptr& geometry, const Material::Ptr& material) noexcept {
		mGeometry = geometry;
		mMaterial = material;

		mIsRenderableObject = true;
	}

	RenderableObject::~RenderableObject() noexcept {}

	auto RenderableObject::onBeforeRender(Renderer* renderer, Scene* scene, Camera* camera) const -> void
	{
		if (mOnBeforeRenderCallback) {
			mOnBeforeRenderCallback(renderer, scene, camera);
		}
	}
}