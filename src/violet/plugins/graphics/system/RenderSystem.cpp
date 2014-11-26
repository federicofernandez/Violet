#include "violet/plugins/graphics/system/RenderSystem.h"

#include "violet/core/entity/Entity.h"
#include "violet/core/math/Circle.h"
#include "violet/core/math/Constants.h"
#include "violet/core/serialization/Deserializer.h"
#include "violet/core/transform/TransformSystem.h"
#include "violet/plugins/graphics/Mesh.h"

#include <GL/glut.h>

using namespace Violet;

namespace RenderSystemNamespace
{
	const char * const ms_componentLabel = "rndr";

	RenderSystem * ms_renderSystem;

	void draw(const RenderComponent & renderComponent);
}

using namespace RenderSystemNamespace;

bool RenderSystem::init(Settings & settings)
{
	if (ms_renderSystem != nullptr)
		return false;

	glutInit(&settings.argc, settings.argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(settings.width, settings.height);
	glutInitWindowPosition(settings.x, settings.y);
	if (glutCreateWindow(settings.title) == 0)
		return false;

	glClearColor(Color::kTan.r, Color::kTan.g, Color::kTan.b, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, settings.width, 0, settings.height);
	glMatrixMode(GL_MODELVIEW);

	ms_renderSystem = new RenderSystem();
	glutDisplayFunc(display);
	return true;
}

void RenderSystem::update(float const /*dt*/)
{
	glutPostRedisplay();
}

void RenderSystem::create(Entity & entity, Deserializer & deserializer)
{
	deserializer.enterSegment(ms_componentLabel);
	ms_renderSystem->m_entityComponentMap.emplace(entity.id, ms_renderSystem->m_components.size());
	ms_renderSystem->m_components.emplace_back(entity, deserializer);
	deserializer.leaveSegment();
}

void RenderSystem::display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto const & component : ms_renderSystem->m_components)
		draw(component);
	glFlush();
}

void RenderSystemNamespace::draw(const RenderComponent & renderComponent)
{
	glPushMatrix();
	const TransformComponent & transform = TransformSystem::fetch(renderComponent.m_entity);
	glTranslatef(transform.m_position.x, transform.m_position.y, 0.f);
	glColor4f(renderComponent.m_color.r, renderComponent.m_color.g, renderComponent.m_color.b, renderComponent.m_color.a);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	const size_t len = renderComponent.m_mesh.m_vertices.size();
	for (size_t i = 0; i < len; i++)
		glVertex2f(renderComponent.m_mesh.m_vertices[i].x, renderComponent.m_mesh.m_vertices[i].y);
	glEnd();
	glPopMatrix();
}