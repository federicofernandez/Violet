// ============================================================================

#include "violet/graphics/component/TextureComponent.h"

#include "violet/Engine.h"
#include "violet/graphics/shader/Shader.h"
#include "violet/math/AABB.h"
#include "violet/math/Polygon.h"
#include "violet/serialization/Deserializer.h"
#include "violet/serialization/Serializer.h"
#include "violet/utility/Guard.h"

#include <GL/glew.h>

using namespace Violet;

// ============================================================================

namespace TextureComponentNamespace
{
	std::vector<Vec2f> createTexCoordsFromMesh(const Mesh & mesh);
}

using namespace TextureComponentNamespace;

// ============================================================================

Tag TextureComponent::getStaticTag()
{
	return Tag('t', 'e', 'x', 'u');
}

// ============================================================================

TextureComponent::TextureComponent(Entity & owner, Deserializer & deserializer) :
	ComponentBase<TextureComponent>(owner),
	RenderComponentData(deserializer),
	m_texture(),
	m_texCoords()
{
	const std::string textureName = deserializer.getString("texture");
	Engine::getInstance().addWriteTask(*this,
		[=](TextureComponent & component)
		{
			component.m_texture = Texture::getCache().fetch(textureName.c_str());
			component.m_texCoords = std::make_unique<Mesh>(createTexCoordsFromMesh(*component.m_mesh));
			glBindVertexArray(component.m_vertexArrayBuffer);
			const Guard<Mesh> texCoordGuard(*component.m_texCoords);
			const GLuint texCoordAttrib = component.m_shader->getAttributeLocation("texCoord");
			glEnableVertexAttribArray(texCoordAttrib);
			glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindVertexArray(0);
		}, Engine::Thread::Window);
}

// ----------------------------------------------------------------------------

TextureComponent::TextureComponent(Entity & owner, const Polygon & poly, std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Texture> texture, const Polygon & texCoords) :
	ComponentBase<TextureComponent>(owner),
	RenderComponentData(poly, shader),
	m_texture(std::move(texture)),
	m_texCoords(std::make_unique<Mesh>(texCoords))
{
}

// ----------------------------------------------------------------------------

TextureComponent::TextureComponent(TextureComponent && other) :
	ComponentBase<TextureComponent>(std::move(other)),
	RenderComponentData(std::move(other)),
	m_texture(),
	m_texCoords(std::move(other.m_texCoords))
{
	m_texture.swap(other.m_texture);
}

// ----------------------------------------------------------------------------

TextureComponent::~TextureComponent()
{
	const auto texCoords = m_texCoords.release();
	Engine::getInstance().addDeleteTask(std::make_unique<DelegateTask>(
		[=]()
		{
			delete texCoords;
		}), Engine::Thread::Window);
}

// ============================================================================

Deserializer & Violet::operator>>(Deserializer & deserializer, TextureComponent & component)
{
	operator>>(deserializer, static_cast<RenderComponentData &>(component));
	component.m_texture = Texture::getCache().fetch(deserializer.getString("texture"));
	return deserializer;
}

// ----------------------------------------------------------------------------

Serializer & Violet::operator<<(Serializer & serializer, const TextureComponent & component)
{
	operator<<(serializer, static_cast<const RenderComponentData &>(component));
	serializer.writeString("texture", component.m_texture->getName().c_str());
	return serializer;
}

// ============================================================================

std::vector<Vec2f> TextureComponentNamespace::createTexCoordsFromMesh(const Mesh & mesh)
{
	AABB extent;
	const auto polygon = mesh.getPolygon();
	for (const auto & vertex : polygon.m_vertices)
		extent.extend(vertex);
	extent.translate(-extent.getMinimum());
	const auto & max = extent.getMaximum();

	std::vector<Vec2f> result;
	result.reserve(polygon.m_vertices.size());
	for (const auto & vertex : polygon.m_vertices)
		result.emplace_back(Vec2f(vertex.x / max.x, vertex.y / max.y));
	return result;
}

	// ============================================================================