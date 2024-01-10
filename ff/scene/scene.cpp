#include "scene.h"

namespace ff
{
	Scene::Ptr Scene::create()
	{
		return std::make_shared<Scene>();
	}

	Scene::Scene() noexcept
	{
		mIsScene = true;
	}

	Scene::~Scene() noexcept = default;
}
