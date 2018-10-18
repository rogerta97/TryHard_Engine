#include "Transform.h"
#include "OpenGL.h"
#include "Application.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::DrawAxis()
{
	LineSegment curr_line;

	glBegin(GL_LINES);
	glDisable(GL_DEPTH_TEST);

	App->renderer3D->UseUIRenderSettings();
	glLineWidth(3.0f);

	//X
	curr_line.a = position; 
	curr_line.b = position + X;

	glColor3f(1.0f, 0.0f, 0.0f); 
	glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
	glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);

	//Y
	curr_line.a = position;
	curr_line.b = position + Y;

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
	glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);

	//Z
	curr_line.a = position;
	curr_line.b = position + Z;

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
	glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
	
	glEnable(GL_DEPTH_TEST);
	glEnd();

	App->renderer3D->UseCurrentRenderSettings(); 
}
