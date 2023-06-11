#pragma once
#include "../global/base.h"

namespace ff {

	class ObjectTypeChecker {
	public:
		bool mIsRenderableObject{ false };
		bool mIsMesh{ false };
		bool mIsSkinnedMesh{ false };
		bool mIsBone{ false };
		bool mIsScene{ false };
		bool mIsCamera{ false };
		bool mIsPerpectiveCamera{ false };
		bool mIsOrthographicCamera{ false };
		bool mIsGroup{ false };
		bool mIsLight{ false };
		bool mIsAmbientLight{ false };
		bool mIsDirectionalLight{ false };
	};

	/// Object3D是所有空间变换、节点结构等的最基础的类型
	class Object3D : public std::enable_shared_from_this<Object3D>, public ObjectTypeChecker {
	public:
		/// 使用智能指针进行管理
		using Ptr = std::shared_ptr<Object3D>;
		static Ptr create() {
			return std::make_shared <Object3D>();
		}

		Object3D() noexcept;

		~Object3D() noexcept;

		/// TODO 满足动态类型转换 
		/// Parent* p = new ChildA();
		/// ChildA* c = p;不行，因为parent可以派生出来ChildA ChildB。。。
		/// ChildA* c = dynamic_cast<ChildA>(p);//static_cast reinterpret_cast 
		/// dynamic动态转换会进行类型检验，如果p并不是ChildA这种对象，那么失败，返回nullptr
		/// dynamic要求父类必须有虚函数
		/// 智能指针也拥有一套类型转换体系，如下：
		/* std::reinterpret_pointer_cast
		std::dynamic_pointer_cast;
		std::static_pointer_cast; */
		virtual auto fakeFunction() noexcept -> void {}

		/// \brief 设置OBJ3D坐标位置 
		/// \param x 
		/// \param y 
		/// \param z 
		auto setPosition(float x, float y, float z) noexcept -> void;

		/// \brief 设置OBJ3D坐标位置
		/// \param position 
		auto setPosition(const glm::vec3& position) noexcept -> void;

		/// \brief 设置旋转矩阵 -> 四元数
		/// \param x 
		/// \param y 
		/// \param z 
		/// \param w 
		auto setQuaternion(float x, float y, float z, float w) noexcept -> void;

		/// \brief 设置缩放
		/// \param x 
		/// \param y 
		/// \param z 
		auto setScale(float x, float y, float z) noexcept -> void;

		/// \brief 绕着模型坐标系的x轴旋转 
		/// \param angle 
		auto rotateX(float angle) noexcept -> void;

		/// \brief 绕着模型坐标系的Y轴旋转 
		/// \param angle 
		auto rotateY(float angle) noexcept -> void;

		/// \brief 绕着模型坐标系的Z轴旋转 
		/// \param angle 
		auto rotateZ(float angle) noexcept -> void;

		/// \brief 将旋转清空，重新旋转
		/// \param axis 
		/// \param angle 
		auto setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept -> void;

		/// \brief 在上一次的基础上持续旋转
		/// \param axis 
		/// \param angle 
		auto rotateAroundAxis(const glm::vec3& axis, float angle) noexcept -> void;

		/// \brief 设置Object3D看向target的变换
		/// \param target 看向的位置
		/// \param up 世界坐标系的up
		auto lookAt(const glm::vec3& target, const glm::vec3& up) noexcept -> void;

		/// \brief 设置Object3D 的模型坐标系（相对于父节点的局部坐标系）
		/// \param localMatrix 
		auto setLocalMatrix(const glm::mat4& localMatrix) noexcept -> void;

		/// \brief 设置模型的世界坐标系
		/// \param worldMatrix 
		auto setWorldMatrix(const glm::mat4& worldMatrix) noexcept -> void;

		/// \brief 向当前的Object3D里面，加入子节点
		/// \param child 子节点
		/// return 防止重复加入和把自己也加进去
		auto addChild(const Object3D::Ptr& child) noexcept -> bool;

		/// \brief 更新Object3D 的模型坐标系
		virtual auto updateMatrix() noexcept -> void;

		/// \brief 更新Object3D 的世界坐标系
		/// \param updateParent 如果有父节点，那么需要做成父节点的worldMatrix，从而把上方所有节点的影响带入 
		/// \param updateChildren 依次更新子节点的worldMatrix
		/// \return 
		virtual auto updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept -> glm::mat4;

		/// \brief	更新Object3D 的View的坐标系
		/// \param viewMatrix 摄像机viewMatrix
		/// \return 
		auto updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept -> glm::mat4;

		/// \brief 更新Object3D 的normal矩阵
		/// \return 
		auto updateNormalMatrix() noexcept -> glm::mat3;

		/// \brief 获得Object3D 的 坐标
		/// \return 
		auto getPosition() const noexcept -> glm::vec3;

		/// \brief 获得Object3D的世界坐标系
		/// \return 
		glm::vec3 getWorldPosition() const noexcept;

		/// \brief 获得Object3D在模型坐标系的朝向
		/// \return 
		auto getLocalDirection() const noexcept -> glm::vec3;

		/// \brief 获得Object3D的世界坐标系的朝向
		/// \return 
		auto getWorldDirection() const noexcept -> glm::vec3;

		/// \brief 获得Object3D的在模型坐标系的up方向
		/// \return 
		auto getUp() const noexcept -> glm::vec3;

		/// \brief 获得Object3D的在模型坐标系的right方向
		/// \return 
		auto getRight() const noexcept -> glm::vec3;

		/// \brief 获得当前OBJ3D的模型坐标系
		/// \return 
		auto getLocalMatrix() const noexcept -> glm::mat4;

		/// \brief 获得当前OBJ3D的世界坐标系
		/// \return 
		auto getWorldMatrix() const noexcept -> glm::mat4;

		/// \brief 获得当前OBJ3D的摄像机坐标系
		/// \return 
		auto getModelViewMatrix() const noexcept -> glm::mat4;

		/// \brief 获得当前OBJ3D的normal矩阵
		/// \return 
		auto getNormalMatrix() const noexcept -> glm::mat3;

		/// \brief 获得当前OBJ3D的所有的子节点
		/// \return 
		auto getChildren() const noexcept -> const std::vector<Object3D::Ptr>&;

		/// \brief 获得OBJ3D的ID
		/// \return 
		auto getID() const noexcept -> ID;

	protected:
		/// \brief从localMatrix中分解出平移 、旋转、缩放 矩阵 
		auto decompose() noexcept -> void;

	public:
		/// visible来表示是否对其进行渲染
		bool	mVisible{ true };

		/// 是否产生阴影
		bool	mCastShadow{ true };

		/// obj的名字
		std::string mName;

		/// 表示是否强制对矩阵进行更新
		bool	mNeedsUpdateMatrix{ true };

	protected:
		ID	mID{ 0 };/// 全局唯一id

		/// for setting easy
		glm::vec3 mPosition{ glm::vec3(0.0f) };

		/// 对于object的旋转变换，我们一律采用四元数
		glm::quat mQuaternion{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f) };

		glm::vec3 mScale{ glm::vec3(1.0f) };

		/// localMatrix对其模型坐标系进行了变换
		glm::mat4	mLocalMatrix = glm::mat4(1.0f);

		/// worldMatrix将模型顶点从模型坐标系，转换到世界坐标系
		glm::mat4	mWorldMatrix = glm::mat4(1.0f);

		/// 保留参数
		bool		mNeedsUpdate{ false };

		/// 节点系统
		/// 父节点采用weakPtr ，防止循环引用
		std::weak_ptr<Object3D>		mParent;
		/// 父节点存储了子节点的sharedPtr，建立一次引用,保证子节点的引用计数至少大于1
		std::vector<Object3D::Ptr>	mChildren{};

		/// for shading
		/// modelViewMatrix将模型顶点，从模型坐标系，转换到当前摄像机坐标系,viewMatrix * worldMatrix
		glm::mat4	mModelViewMatrix = glm::mat4(1.0f);

		/// 将模型的normal从模型坐标系，转换到摄像机坐标系
		glm::mat3	mNormalMatrix = glm::mat3(1.0f);
	};
}