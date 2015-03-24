#ifndef SHADER_H
#define SHADER_H

#include "violet/core/utility/ResourceCache.h"

#include <GL/glew.h>
#include <memory>

namespace Violet
{
	class Shader
	{
	public:

		friend class ShaderProgram;

		enum Type
		{
			Vertex,
			Fragment
		};

	public:

		static std::unique_ptr<Shader> create(const char * filename, Type type);

	public:

		~Shader();

	private:

		Shader(GLuint handle);

	private:

		const GLuint m_handle;
	};

	class ShaderProgram
	{
	public:

		static std::unique_ptr<ShaderProgram> load(const char * name);
		static std::unique_ptr<ShaderProgram> load(const char * vertexShaderFilename, const char * fragmentShaderFilename);
		static std::unique_ptr<ShaderProgram> create(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
		typedef ResourceCache<ShaderProgram, &ShaderProgram::load> Cache;
		static Cache & getCache();
		static void bind(const ShaderProgram & program);
		static void unbind();

	public:

		ShaderProgram(GLuint handle, std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
		~ShaderProgram();

		int getAttributeLocation(const char * name);
		int getUniformLocation(const char * name);

	private:

		const GLuint m_handle;
		const std::shared_ptr<Shader> m_vertexShader;
		const std::shared_ptr<Shader> m_fragmentShader;
	};
}

#endif