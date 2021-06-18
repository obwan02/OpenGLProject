#pragma once
#include <oglpch.h>

namespace ogl {

	enum class ShaderDataType
	{
		Bool,
		Mat3, Mat4,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4
	};

	GLenum ShaderDataTypeToGLType(ShaderDataType t) {
		switch (t) {
		case ShaderDataType::Bool: return GL_BOOL;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4: return GL_INT;
		}

		OGL_ASSERT(false, "Invalid ShaderDataType specified");
	}
	GLenum ShaderDataTypeToComponentCount(ShaderDataType t) {
		switch (t) {
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::Mat3: return 3; // we handle the case of a Matrices in VertexArray
		case ShaderDataType::Mat4: return 4;
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Int2: return 2;
		case ShaderDataType::Int3: return 3;
		case ShaderDataType::Int4: return 4;
		}

		OGL_ASSERT(false, "Invalid ShaderDataType specified");
	}

}