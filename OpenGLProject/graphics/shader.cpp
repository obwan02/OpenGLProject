#include "shader.h"
#include <optional>

namespace ogl {


	Shader::Shader(uint32_t id) : m_ProgramId(id) {}
	Shader::Shader(Shader&& other) : m_ProgramId(other.m_ProgramId) { other.m_ProgramId = 0; }
	Shader::~Shader() { if(m_ProgramId) glDeleteProgram(m_ProgramId); }

	Shader& Shader::operator=(Shader&& other) {
		std::swap(m_ProgramId, other.m_ProgramId);
		return *this;
	}

	void Shader::bind() { glUseProgram(m_ProgramId); }
	void Shader::unbind() { glUseProgram(0); }

	void ShaderBuilder::add_vertex_shader(FILE* file) {
		if(const auto data = ReadFile(file))
			add_vertex_shader(*data);
		else 
			log::ErrorFrom("ShaderBuilder", "Failed to read vertex shader file.");
	}

	void ShaderBuilder::add_vertex_shader(std::string_view source) {
		const uint32_t id = glCreateShader(GL_VERTEX_SHADER);
		if(!id) {
			log::ErrorFrom("ShaderBuilder", "Failed to create vertex shader because of an internal GL error");
			return;
		}

		const int32_t size = source.size(); 
		const char* data = source.data();
		glShaderSource(id, 1, &data, &size);
		m_ShaderIds.push_back(id);
	}


	void ShaderBuilder::add_fragment_shader(FILE* file) {
		if(const auto data = ReadFile(file))
			add_fragment_shader(*data);
		else 
			log::ErrorFrom("ShaderBuilder", "Failed to read fragment shader file");
	}

	void ShaderBuilder::add_fragment_shader(std::string_view source) {
		const uint32_t id = glCreateShader(GL_FRAGMENT_SHADER);
		if(!id) {
			log::ErrorFrom("ShaderBuilder", "Failed to create fragment shader because of an internal GL error");
			return;
		}

		const int32_t size = source.size(); 
		const char* data = source.data();
		glShaderSource(id, 1, &data, &size);
		m_ShaderIds.push_back(id);
	}

	void ShaderBuilder::specify_attrib(uint32_t id, const std::string& name) {
		m_Attribs.push_back({id, name});
	}

	std::optional<Shader> ShaderBuilder::generate() {
		if(m_ShaderIds.size() == 0){
			log::ErrorFrom("Shader Builder", "Shader failed to generate because there were no shaders");
			return std::nullopt;
		}

		const uint32_t progId = glCreateProgram();
		if(!progId) {
			log::ErrorFrom("ShaderBuilder", "Shader failed to generate because of an internal GL error");
			return std::nullopt;
		}

		// We store all the shaders we successfully managed to attach in this vector.
		ArrayVector<uint32_t, MAX_SHADERS> attachedShaders;

		bool success = false;
		// When the functions returns this will be called
		SCOPE_DEFER([&] {
			for(const auto& id : m_ShaderIds) {
				glDeleteShader(id);
			}

			for(const auto& id : attachedShaders) {
				glDetachShader(progId, id);
			}

			// Delete the program if we didn't succeed in creating it.
			if(!success) glDeleteProgram(progId);
		});


		// Compile individual shader parts
		for(const auto& id : m_ShaderIds) {
			glCompileShader(id);

			int32_t compileStatus = GL_FALSE;
			glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
			
			if(!compileStatus) {
				int32_t logLength;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
				
				char* log = new char[logLength + 1];
				glGetShaderInfoLog(id, logLength, NULL, log);
				log[logLength] = '\0';

				log::ErrorFrom("ShaderBuilder", "Shader failed to compile: ", log);
				return std::nullopt;
			}

			glAttachShader(progId, id);
			attachedShaders.push_back(id);
		}

		// Specify attributes
		for(auto&[id, name] : m_Attribs) {
			glBindAttribLocation(progId, id, name.c_str());
		}

		glLinkProgram(progId);

		int32_t linkStatus = GL_FALSE;
		glGetProgramiv(progId, GL_LINK_STATUS, &linkStatus);
		
		if(!linkStatus) {	
			int32_t logLength;
			glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &logLength);
			
			char* log = new char[logLength + 1];
			glGetProgramInfoLog(progId, logLength, NULL, log);
			log[logLength] = '\0';

			log::ErrorFrom("ShaderBuilder", "Shader failed to link: ", log);
			glDeleteProgram(progId);
			return std::nullopt;
		}
		
		success = true;
		return Shader(progId);
	}
}
