#include "object3D.h"
#include "../tools/identity.h"
#include "../global/eventDispatcher.h"

namespace ff
{
	Object3D::Ptr Object3D::create()
	{
		return std::make_shared <Object3D>();
	}

	auto Object3D::fakeFunction() noexcept -> void
	{
		
	}

	Object3D::Object3D() noexcept
	{
		mID = Identity::generateID();
	}

	Object3D::~Object3D() noexcept
	{
		EventBase::Ptr e = EventBase::create("objectDispose");
		e->mTarget = this;
		EventDispatcher::getInstance()->dispatchEvent(e);
	}

	auto Object3D::setPosition(float x, float y, float z) noexcept -> void
	{
		setPosition(glm::vec3(x, y, z));
	}

	auto Object3D::setPosition(const glm::vec3& position) noexcept -> void
	{
		/// 列优先
		mLocalMatrix[3].x = position.x;
		mLocalMatrix[3].y = position.y;
		mLocalMatrix[3].z = position.z;

		mPosition = position;
	}

	auto Object3D::setQuaternion(float x, float y, float z, float w) noexcept -> void
	{
		/// 在四元数情况下，glm的初始化，w xyz
		glm::quat quaternion(w, x, y, z);

		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

		/// 将glm的四元数转换为一个旋转矩阵
		glm::mat4 rotateMatrix = glm::mat4_cast(quaternion);

		/// 将scale变换恢复进去
		mLocalMatrix[0] = rotateMatrix[0] * scaleX;
		mLocalMatrix[1] = rotateMatrix[1] * scaleY;
		mLocalMatrix[2] = rotateMatrix[2] * scaleX;

		/// 基于现在的参数，推导其他的参数
		decompose();
	}

	auto Object3D::setScale(float x, float y, float z) noexcept -> void
	{
		/// 拿到某一列，normalize去掉之前的scale影响,再乘以当前的相关scale
		auto col0 = glm::normalize(glm::vec3(mLocalMatrix[0])) * x;
		auto col1 = glm::normalize(glm::vec3(mLocalMatrix[1])) * y;
		auto col2 = glm::normalize(glm::vec3(mLocalMatrix[2])) * z;

		/// 将设置好的前三列，重新设置到localmatrix
		mLocalMatrix[0] = glm::vec4(col0, 0.0f);
		mLocalMatrix[1] = glm::vec4(col1, 0.0f);
		mLocalMatrix[2] = glm::vec4(col2, 0.0f);

		/// 推导其他参数
		decompose();
	}


	auto Object3D::rotateX(float angle) noexcept -> void
	{
		/// 首先获取到当前模型状态下的右侧方向
		glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[0]);

		/// 针对这个右侧方向作为旋转轴来进行旋转,angle正负有意义，当angle>0, 当你冲着旋转轴看，逆时针旋转
		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
		mLocalMatrix = rotateMatrix * mLocalMatrix;

		decompose();
	}

	auto Object3D::rotateY(float angle) noexcept -> void
	{
		glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[1]);
		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
		mLocalMatrix = rotateMatrix * mLocalMatrix;

		decompose();
	}

	auto Object3D::rotateZ(float angle) noexcept -> void
	{
		glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[2]);
		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
		mLocalMatrix = rotateMatrix * mLocalMatrix;

		decompose();
	}

	auto Object3D::rotateAroundAxis(const glm::vec3& axis, float angle) noexcept -> void
	{
		mLocalMatrix = glm::rotate(mLocalMatrix, glm::radians(angle), axis);

		decompose();
	}

	auto Object3D::setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept -> void
	{
		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), axis);

		/// decompose scale
		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scaleX, scaleY, scaleZ));

		/// 列相等
		mLocalMatrix[0] = rotateMatrix[0];
		mLocalMatrix[1] = rotateMatrix[1];
		mLocalMatrix[2] = rotateMatrix[2];

		mLocalMatrix *= scaleMatrix;

		decompose();
	}

	auto Object3D::lookAt(const glm::vec3& target, const glm::vec3& up) noexcept -> void
	{
		/// decompose scale
		float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
		float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
		float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

		glm::vec3 position = glm::vec3(mLocalMatrix[3]);

		/// make local coordinate
		auto nTarget = glm::normalize(target - position) * scaleZ;
		auto nRight = glm::normalize(glm::cross(up, -nTarget)) * scaleX;
		auto nUp = glm::normalize(glm::cross(nRight, nTarget)) * scaleY;


		mLocalMatrix[0] = glm::vec4(nRight, 0.0f);
		mLocalMatrix[1] = glm::vec4(nUp, 0.0f);
		mLocalMatrix[2] = glm::vec4(-nTarget, 0.0f);
		mLocalMatrix[3] = glm::vec4(position, 1.0f);

		decompose();
	}

	auto Object3D::setLocalMatrix(const glm::mat4& localMatrix) noexcept -> void
	{
		mLocalMatrix = localMatrix;

		decompose();
	}

	auto Object3D::setWorldMatrix(const glm::mat4& worldMatrix) noexcept -> void
	{
		mWorldMatrix = worldMatrix;
	}

	auto Object3D::updateMatrix() noexcept -> void
	{
		if (mNeedsUpdateMatrix)
		{
			mNeedsUpdateMatrix = false;
			auto translateMatrix = glm::translate(glm::mat4(1.0f), mPosition);
			auto rotateMatrix = glm::mat4_cast(mQuaternion);
			auto scaleMatrix = glm::scale(glm::mat4(1.0f), mScale);

			mLocalMatrix = translateMatrix * rotateMatrix * scaleMatrix;
		}
	}

	/// 通过层级matrix相乘，得到最后的转换到世界坐标系的矩阵
	auto Object3D::updateWorldMatrix(bool updateParent, bool updateChildren) noexcept -> glm::mat4
	{
		/// 检查有没有父节点
		if (!mParent.expired() && updateParent)
		{
			const auto parent = mParent.lock(); /// 拿到父节点的sharedPtr
			parent->updateWorldMatrix(true, false); ///调用父节点的worldMatrix升级更新
		}

		/// 更新自己的localMatrix 
		updateMatrix();

		/// 初始化worldMatrix，如果没有父节点，那么二者相等
		mWorldMatrix = mLocalMatrix;

		/// 如果有父节点，那么需要做成父节点的worldMatrix，从而把上方所有节点的影响带入
		if (!mParent.expired())
		{
			auto parent = mParent.lock();
			mWorldMatrix = parent->mWorldMatrix * mWorldMatrix;
		}

		/// 依次更新子节点的worldMatrix
		if (updateChildren)
		{
			for (auto& child : mChildren)
			{
				child->updateWorldMatrix(false, true);
			}
		}

		return mWorldMatrix;
	}

	/// 传入摄像机viewMatrix
	auto Object3D::updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept -> glm::mat4
	{
		mModelViewMatrix = viewMatrix * mWorldMatrix;

		return mModelViewMatrix;
	}

	auto Object3D::updateNormalMatrix() noexcept -> glm::mat3
	{
		/// normalMatrix 由于存在scale的影响，不能直接变换，否则normal会无法保证垂直于表面
		mNormalMatrix = glm::transpose(glm::inverse(glm::mat3(mModelViewMatrix)));

		return mNormalMatrix;
	}

	auto Object3D::getPosition() const noexcept -> glm::vec3
	{
		return glm::vec3(mLocalMatrix[3]);
	}

	auto Object3D::getWorldPosition() const noexcept -> glm::vec3
	{
		return glm::vec3(mWorldMatrix[3]);
	}

	auto Object3D::getLocalDirection() const noexcept -> glm::vec3
	{
		return glm::normalize(-glm::vec3(mLocalMatrix[2]));
	}

	auto Object3D::getWorldDirection() const noexcept -> glm::vec3
	{
		return glm::normalize(-glm::vec3(mWorldMatrix[2]));
	}

	auto Object3D::getUp() const noexcept -> glm::vec3
	{
		return glm::normalize(glm::vec3(mLocalMatrix[1]));
	}

	auto Object3D::getRight() const noexcept -> glm::vec3
	{
		return glm::normalize(glm::vec3(mLocalMatrix[0]));
	}

	auto Object3D::getLocalMatrix() const noexcept -> glm::mat4
	{
		return mLocalMatrix;
	}

	auto Object3D::getWorldMatrix() const noexcept -> glm::mat4
	{
		return mWorldMatrix;
	}

	glm::mat4 Object3D::getModelViewMatrix() const noexcept
	{
		return mModelViewMatrix;
	}

	auto Object3D::getNormalMatrix() const noexcept -> glm::mat3
	{
		return mNormalMatrix;
	}


	auto Object3D::addChild(const Object3D::Ptr& child) noexcept -> bool
	{
		if (child == shared_from_this())
		{
			return false;
		}

		child->mParent = shared_from_this();

		auto iter = std::find(mChildren.begin(), mChildren.end(), child);
		if (iter != mChildren.end())
		{
			return false;
		}

		mChildren.push_back(child);

		return true;
	}

	auto Object3D::getChildren() const noexcept -> const std::vector<Object3D::Ptr>&
	{
		return mChildren;
	}

	auto Object3D::getID() const noexcept -> ID
	{
		return mID;
	}

	auto Object3D::decompose() noexcept -> void
	{
		glm::vec3 skew;
		glm::vec4 perspective;

		/// 是将变换矩阵当中的参数们，抽离出来 
		glm::decompose(mLocalMatrix, mScale, mQuaternion, mPosition, skew, perspective);
	}
}
