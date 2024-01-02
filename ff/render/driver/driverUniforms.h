#pragma once
#include "../../global/base.h"
#include "../../global/constant.h"
#include "driverTextures.h"
#include "../shaders/uniformsLib.h"

namespace ff {

	class DriverUniforms;

	/// 一切uniform类型的根类
	class UniformBase {
	public:
		using Ptr = std::shared_ptr<UniformBase>;

		UniformBase() noexcept;

		~UniformBase() noexcept;

		/// \brief uniform的名字
		std::string mID;

		
		/// \brief 更新uniform
		/// \param value 
		/// \param textures 
		/// \param driverUniforms 
		virtual auto setValue(
			const std::any& value,
			const DriverTextures::Ptr& textures,
			const std::shared_ptr<DriverUniforms>& driverUniforms) -> void {}
	};

	/// 1 存储location type类型
	/// 2 提供setValue的接口，并且在其内部，根据不同类型的uniform调用不同的upload

	class SingleUniform : public UniformBase {
	public:
		using Ptr = std::shared_ptr<SingleUniform>;
		static Ptr create(const std::string& id, const GLint& location, const GLenum& type) {
			return std::make_shared<SingleUniform>(id, location, type);
		}

		SingleUniform(const std::string& id, const GLint& location, const GLenum& type) noexcept;

		~SingleUniform() noexcept;

	public:
		GLint		mLocation{ 0 };
		GLenum		mType;

	public:

		auto setValue(
			const std::any& value,
			const DriverTextures::Ptr& textures,
			const std::shared_ptr<DriverUniforms>& driverUniforms) -> void override;

		auto uploadTexture(
			const std::shared_ptr<DriverUniforms>& driverUniforms,
			const DriverTextures::Ptr& textures,
			const std::any& value) -> void;

		
		/// \brief 将T类型的变量value，传输到location所代表的uniform里面
		/// \tparam T 
		/// \param value 
		template<typename T>
		auto upload(const T& value) -> void {}

		/// 模板偏特化
		
		template<>
		auto upload<float>(const float& value) -> void;

		template<>
		auto upload<glm::vec2>(const glm::vec2& value) -> void;

		template<>
		auto upload<glm::vec3>(const glm::vec3& value) -> void;

		template<>
		auto upload<glm::vec4>(const glm::vec4& value) -> void;

		template<>
		auto upload<int>(const int& value) -> void;

		template<>
		auto upload<glm::ivec2>(const glm::ivec2& value) -> void;

		template<>
		auto upload<glm::ivec3>(const glm::ivec3& value) -> void;

		template<>
		auto upload<glm::ivec4>(const glm::ivec4& value) -> void;

		template<>
		auto upload<bool>(const bool& value) -> void;

		template<>
		auto upload<glm::bvec2>(const glm::bvec2& value) -> void;

		template<>
		auto upload<glm::bvec3>(const glm::bvec3& value) -> void;

		template<>
		auto upload<glm::bvec4>(const glm::bvec4& value) -> void;

		template<>
		auto upload<glm::mat2>(const glm::mat2& value) -> void;

		template<>
		auto upload<glm::mat3>(const glm::mat3& value) -> void;

		template<>
		auto upload<glm::mat4>(const glm::mat4& value) -> void;

	};

	/// 纯粹的数组类型的Uniform
	class PureArrayUniform : public UniformBase {
	public:
		using Ptr = std::shared_ptr<PureArrayUniform>;
		static Ptr create(const std::string& id, const GLint& location, const GLenum& type, GLint size) {
			return std::make_shared<PureArrayUniform>(id, location, type, size);
		}

		PureArrayUniform(const std::string& id, const GLint& location, const GLenum& type, GLint size) noexcept;

		~PureArrayUniform() noexcept;

		GLint		mLocation{ 0 };
		GLenum		mType;
		GLint		mSize{ 0 };

	public:
		auto setValue(
			const std::any& value,
			const DriverTextures::Ptr& textures,
			const std::shared_ptr<DriverUniforms>& driverUniforms) -> void override;

		void uploadTexture2DArray(
			const std::shared_ptr<DriverUniforms>& driverUniforms,
			const DriverTextures::Ptr& textures,
			const std::any& value);

		template<typename T>
		auto upload(const T* value) -> void;

		template<>
		auto upload<float>(const float* value) -> void;

		template<>
		auto upload<glm::vec2>(const glm::vec2* value) -> void;

		template<>
		auto upload<glm::vec3>(const glm::vec3* value) -> void;

		template<>
		auto upload<glm::vec4>(const glm::vec4* value) -> void;

		template<>
		auto upload<int>(const int* value) -> void;

		template<>
		auto upload<glm::ivec2>(const glm::ivec2* value) -> void;

		template<>
		auto upload<glm::ivec3>(const glm::ivec3* value) -> void;

		template<>
		auto upload<glm::ivec4>(const glm::ivec4* value) -> void;

		/// no bool bvec2 bvec3 bvec4 we use int instead

		template<>
		auto upload<glm::mat2>(const glm::mat2* value) -> void;

		template<>
		auto upload<glm::mat3>(const glm::mat3* value) -> void;

		template<>
		auto upload<glm::mat4>(const glm::mat4* value) -> void;

	};

	template <typename T>
	auto PureArrayUniform::upload(const T* value) -> void
	{}

	class UniformContainer {
	public:
		using Ptr = std::shared_ptr<UniformContainer>;
		std::unordered_map<std::string, UniformBase::Ptr> mUniformMap;

		virtual void fake() {}
	};

	class StructuredUniform :public UniformBase, public UniformContainer {
	public:
		using Ptr = std::shared_ptr<StructuredUniform>;
		static Ptr create(const std::string& id) {
			return std::make_shared<StructuredUniform>(id);
		}

		StructuredUniform(const std::string& id) noexcept;

		~StructuredUniform() noexcept;

		auto setValue(
			const std::any& value,
			const DriverTextures::Ptr& textures,
			const std::shared_ptr<DriverUniforms>& driverUniforms) -> void override;
	};

	class DriverUniforms :public UniformContainer, public std::enable_shared_from_this<DriverUniforms> {
	public:
		using Ptr = std::shared_ptr<DriverUniforms>;
		static Ptr create(const GLint& program) { return std::make_shared<DriverUniforms>(program); }

		DriverUniforms(const GLint& program) noexcept;

		~DriverUniforms() noexcept;

		auto upload(UniformHandleMap& uniformHandleMap, const DriverTextures::Ptr& textures) -> void;

		auto addUniform(UniformContainer* container, const UniformBase::Ptr& uniformObject) -> void;

		
		/// \brief texture slots
		/// \param location 
		/// \param slot 
		auto setTextureSlot(const GLint& location, GLuint slot) noexcept -> void;

		auto getTextureSlot(const GLint& location) noexcept -> GLint;


		/// \brief 跟Texture Array相关 uniform sampler2D texs[10]
		/// \param location 
		/// \param slot 
		auto setTextureArraySlot(const GLint& location, std::vector<GLint> slot) noexcept -> void;

		auto getTextureArraySlot(const GLint& location) noexcept -> std::vector<GLint>;

		/// \brief 返回n个可以使用的textureUnitss
		/// \param n 
		/// \return 
		auto allocateTextureUnits(const int& n) -> std::vector<GLint>;

	private:
		/// key:某一个uniform sampler2D tex;变量的location
		/// svalue：GL_TEXTUREXXXX
		std::unordered_map<GLint, GLuint> mTextureSlots{};

		/// uniform sampler2D texs[10];
		/// name: texs[0] 
		/// size:10
		/// 需要给texs分配十个slot（units）
		/// 每个purearrayuniform只有一个location
		/// key：PureArrayUniform这种类型的texture数组的location 
		/// value：数组，为这个texture数组当中的所有textures按序分配的textureUnits
		std::unordered_map<GLint, std::vector<GLint>> mTextureArraySlots{};

		/// 用于记录当前已经分配到了哪一个TextureUnit
		GLint	mCurrentTextureSlots{ 0 };
	};

	template<>
	inline auto SingleUniform::upload<float>(const float& value) -> void
	{
		glUniform1f(mLocation, value);
	}

	template<>
	inline auto SingleUniform::upload<glm::vec2>(const glm::vec2& value) -> void
	{
		/// 如果要从glm：：vec2这个类型的变量，拿出来其数据指针，就得使用glm::value_ptr
		/// 这是为了展示多种多样的做法
		///	glUniform2f(mLocation, value.x, value.y);
		glUniform2fv(mLocation, 1, glm::value_ptr(value));
	}

	template<>
	inline auto SingleUniform::upload<glm::vec3>(const glm::vec3& value) -> void
	{
		glUniform3fv(mLocation, 1, glm::value_ptr(value));
	}

	template<>
	inline auto SingleUniform::upload<glm::vec4>(const glm::vec4& value) -> void
	{
		glUniform4fv(mLocation, 1, glm::value_ptr(value));
	}

	template<>
	inline void SingleUniform::upload<int>(const int& value) {
		glUniform1i(mLocation, value);
	}

	template<>
	inline auto SingleUniform::upload<glm::ivec2>(const glm::ivec2& value) -> void
	{
		glUniform2i(mLocation, value.x, value.y);
	}

	template<>
	inline auto SingleUniform::upload<glm::ivec3>(const glm::ivec3& value) -> void
	{
		glUniform3i(mLocation, value.x, value.y, value.z);
	}

	template<>
	inline auto SingleUniform::upload<glm::ivec4>(const glm::ivec4& value) -> void
	{
		glUniform4i(mLocation, value.x, value.y, value.z, value.w);
	}

	template<>
	inline auto SingleUniform::upload<bool>(const bool& value) -> void
	{
		int v = value;
		glUniform1i(mLocation, v);
	}

	template<>
	inline auto SingleUniform::upload<glm::bvec2>(const glm::bvec2& value) -> void
	{
		glm::ivec2 v = value;
		glUniform2i(mLocation, v.x, v.y);
	}

	template<>
	inline auto SingleUniform::upload<glm::bvec3>(const glm::bvec3& value) -> void
	{
		glm::ivec3 v = value;
		glUniform3i(mLocation, v.x, v.y, v.z);
	}

	template<>
	inline auto SingleUniform::upload<glm::bvec4>(const glm::bvec4& value) -> void
	{
		glm::ivec4 v = value;
		glUniform4i(mLocation, v.x, v.y, v.z, v.w);
	}

	template<>
	inline auto SingleUniform::upload<glm::mat2>(const glm::mat2& value) -> void
	{
		glUniformMatrix2fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	template<>
	inline auto SingleUniform::upload<glm::mat3>(const glm::mat3& value) -> void
	{
		glUniformMatrix3fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	template<>
	inline auto SingleUniform::upload<glm::mat4>(const glm::mat4& value) -> void
	{
		glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	template<>
	inline auto PureArrayUniform::upload<float>(const float* value) -> void
	{
		glUniform1fv(mLocation, mSize, value);
	}

	template<>
	inline auto PureArrayUniform::upload<glm::vec2>(const glm::vec2* value) -> void
	{
		/// 需要拿到数组开头的指针，拿到数组的第一个元素 value【0】，然后取其数据地址
		glUniform2fv(mLocation, mSize, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::vec3>(const glm::vec3* value) -> void
	{
		glUniform3fv(mLocation, mSize, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::vec4>(const glm::vec4* value) -> void
	{
		glUniform4fv(mLocation, mSize, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<int>(const int* value) -> void
	{
		glUniform1iv(mLocation, mSize, value);
	}

	template<>
	inline auto PureArrayUniform::upload<glm::ivec2>(const glm::ivec2* value) -> void
	{
		glUniform2iv(mLocation, mSize, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::ivec3>(const glm::ivec3* value) -> void
	{
		glUniform3iv(mLocation, mSize, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::ivec4>(const glm::ivec4* value) -> void
	{
		glUniform4iv(mLocation, mSize, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::mat2>(const glm::mat2* value) -> void
	{
		glUniformMatrix2fv(mLocation, mSize, GL_FALSE, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::mat3>(const glm::mat3* value) -> void
	{
		glUniformMatrix3fv(mLocation, mSize, GL_FALSE, glm::value_ptr(value[0]));
	}

	template<>
	inline auto PureArrayUniform::upload<glm::mat4>(const glm::mat4* value) -> void
	{
		glUniformMatrix4fv(mLocation, mSize, GL_FALSE, glm::value_ptr(value[0]));
	}

}