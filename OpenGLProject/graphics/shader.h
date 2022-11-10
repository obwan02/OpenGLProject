#pragma once

#include <any>
#include <cstdio>
#include <string>
#include <optional>
#include <istream>
#include <string_view>

#include <glad/glad.h>
#include <unordered_map>

#include "core.h"
#include "defer.h"
#include "util/fileio.h"
#include "util/array_vector.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "graphics/texture.h"

#define MAX_SHADERS 10
#define MAX_ATTRIBS 16

namespace ogl {
	
	class Shader {
	private:
		friend class ShaderBuilder;
		Shader(uint32_t id);

	public:
		Shader(Shader& other) = delete;
		Shader(Shader&& other);
		Shader& operator=(Shader&& shader);
		~Shader();
		
		void bind();
		void unbind();

		// TODO: Make uniform assignment better
		template<typename T>
		void set_uniform(const char* name, const T& value) {
			bind();
			auto id = glGetUniformLocation(m_ProgramId, name);
			OGL_DEBUG_ASSERT(id != -1, "Uniform name is invalid");
			set_uniform_gl<T>(id, value);
			unbind();
		}

		template<typename T>
		void set_uniform_array(const char* name, const T* values, size_t count) {
			bind();
			auto id = glGetUniformLocation(m_ProgramId, name);
			OGL_DEBUG_ASSERT(id != -1, "Uniform name invalid");
			set_uniform_arr_gl<T>(id, values, count);
			unbind();
		}

		uint32_t get_renderer_id() { return m_ProgramId; }
	private:
		template<typename T> void set_uniform_gl(uint32_t id, const T& value);
		template<typename T> void set_uniform_arr_gl(uint32_t id, const T* value, size_t count);
	private:
		uint32_t m_ProgramId;
	};

	typedef std::unordered_map<std::string, std::any> ShaderVars;

	class ShaderBuilder {
	public:
		ShaderBuilder();
		ShaderBuilder(ShaderVars&& vars);

		// Adds a vertex shader by reading from a file.
		// This function will fully read the FILE* stream.
		void add_vertex_shader(FILE* sourceFile);
		// Adds a vertex shader from the source data
		// inside source. SOURCE IS NOT A PATH.
		void add_vertex_shader(std::string_view source);
		// Adds a vertex shader by reading from a file.
		// This function will fully read the FILE* stream.
		void add_fragment_shader(FILE* sourceFile);
		// Adds a fragment shader from the source data
		// inside source. SOURCE IS NOT A PATH.
		void add_fragment_shader(std::string_view source);
		//TODO: Add other types of shader parts

		// Specifies an attributes index. The index refers to the same 
		// index used in VertexArray.set_attrib(index, ...). The name is the
		// name of the attribute in the GLSL shader. 
		void specify_attrib(uint32_t id, const std::string& name);

		std::optional<Shader> generate();
	
	private:

		std::string replace_vars(std::string_view);

	private:
		ShaderVars m_Vars;
		ArrayVector<uint32_t, MAX_SHADERS> m_ShaderIds;
		ArrayVector<std::pair<uint32_t, std::string>, MAX_ATTRIBS> m_Attribs;
	};

	template<> inline void Shader::set_uniform_gl<int32_t>(uint32_t id, const int32_t&  value) { glUniform1i(id, value); }
	template<> inline void Shader::set_uniform_gl<Vector2<int32_t>>(uint32_t id, const Vector2<int32_t>& value) { glUniform2i(id, value.x, value.y); }
	template<> inline void Shader::set_uniform_gl<Vector3<int32_t>>(uint32_t id, const Vector3<int32_t>& value) { glUniform3i(id, value.x, value.y, value.z); }
	template<> inline void Shader::set_uniform_gl<Vector4<int32_t>>(uint32_t id, const Vector4<int32_t>& value) { glUniform4i(id, value.x, value.y, value.z, value.w); }
	template<> inline void Shader::set_uniform_gl<uint32_t>(uint32_t id, const uint32_t& value) { glUniform1ui(id, value); }
	template<> inline void Shader::set_uniform_gl<Vector2<uint32_t>>(uint32_t id, const Vector2<uint32_t>& value) { glUniform2ui(id, value.x, value.y); }
	template<> inline void Shader::set_uniform_gl<Vector3<uint32_t>>(uint32_t id, const Vector3<uint32_t>& value) { glUniform3ui(id, value.x, value.y, value.z); }
	template<> inline void Shader::set_uniform_gl<Vector4<uint32_t>>(uint32_t id, const Vector4<uint32_t>& value) { glUniform4ui(id, value.x, value.y, value.z, value.w); }
	template<> inline void Shader::set_uniform_gl<float>(uint32_t id, const float& value) { glUniform1f(id, value); } 
	template<> inline void Shader::set_uniform_gl<Vector2<float>>(uint32_t id, const Vector2<float>& value) { glUniform2f(id, value.x, value.y); }
	template<> inline void Shader::set_uniform_gl<Vector3<float>>(uint32_t id, const Vector3<float>& value) { glUniform3f(id, value.x, value.y, value.z); }
	template<> inline void Shader::set_uniform_gl<Vector4<float>>(uint32_t id, const Vector4<float>& value) { glUniform4f(id, value.x, value.y, value.z, value.w); }

	template<> inline void Shader::set_uniform_arr_gl<int32_t>(uint32_t id, const int32_t* value, size_t count) { glUniform1iv(id, count, value); }
	template<> inline void Shader::set_uniform_arr_gl<Vector2<int32_t>>(uint32_t id, const Vector2<int32_t>* value, size_t count) { glUniform2iv(id, count, &value->x); }
	template<> inline void Shader::set_uniform_arr_gl<Vector3<int32_t>>(uint32_t id, const Vector3<int32_t>* value, size_t count) { glUniform3iv(id, count, &value->x); }
	template<> inline void Shader::set_uniform_arr_gl<Vector4<int32_t>>(uint32_t id, const Vector4<int32_t>* value, size_t count) { glUniform4iv(id, count, &value->x); }
	template<> inline void Shader::set_uniform_arr_gl<uint32_t>(uint32_t id, const uint32_t* value, size_t count) { glUniform1uiv(id, count, value); }
	template<> inline void Shader::set_uniform_arr_gl<Vector2<uint32_t>>(uint32_t id, const Vector2<uint32_t>* value, size_t count) { glUniform2uiv(id, count, &value->x); }
	template<> inline void Shader::set_uniform_arr_gl<Vector3<uint32_t>>(uint32_t id, const Vector3<uint32_t>* value, size_t count) { glUniform3uiv(id, count, &value->x); }
	template<> inline void Shader::set_uniform_arr_gl<Vector4<uint32_t>>(uint32_t id, const Vector4<uint32_t>* value, size_t count) { glUniform4uiv(id, count, &value->x); }
	template<> inline void Shader::set_uniform_arr_gl<float>(uint32_t id, const float* value, size_t count) { glUniform1fv(id, count, value); } 
	template<> inline void Shader::set_uniform_arr_gl<Vector2<float>>(uint32_t id, const Vector2<float>* value, size_t count) { glUniform2fv(id, count, &value->x); } 
	template<> inline void Shader::set_uniform_arr_gl<Vector3<float>>(uint32_t id, const Vector3<float>* value, size_t count) { glUniform3fv(id, count, &value->x); } 
	template<> inline void Shader::set_uniform_arr_gl<Vector4<float>>(uint32_t id, const Vector4<float>* value, size_t count) { glUniform4fv(id, count, &value->x); } 

	template<> inline void Shader::set_uniform_gl<Matrix4<float>>(uint32_t id, const Matrix4<float>& value) { glUniformMatrix4fv(id, 1, true, value.data); }
	template<> inline void Shader::set_uniform_arr_gl<Matrix4<float>>(uint32_t id, const Matrix4<float>* value, size_t count) { glUniformMatrix4fv(id, count, true, value->data); }
}
