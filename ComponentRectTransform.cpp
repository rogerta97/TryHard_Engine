#include "ComponentRectTransform.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "UI_Label.h"
#include "ComponentText.h"
#include "UI_GamePanel.h"
#include "DebugDraw.h"
#include "ComponentMesh.h"
#include "ComponentCanvasScaler.h"
#include "Mesh.h"
#include "OpenGL.h"
#include "ModuleUserInterface.h"

#define CANVAS_FONT_REL 6

ComponentRectTransform::ComponentRectTransform(GameObject* parent)
{
	component_type = CMP_RECTTRANSFORM;
	gameobject = parent;

	transform_part = new ComponentTransform(parent);

	transform_part->transform.position = float3::zero;
	transform_part->transform.rotation = Quat::identity;
	transform_part->transform.scale = { 1,1,1 };

	scale_to_show = transform_part->transform.scale; 

	anchor.min_x = 0.5;
	anchor.min_y = 0.5;
	anchor.max_x = 0.5;
	anchor.max_y = 0.5;

	relative_pos.x = 0;
	relative_pos.y = 0;

	GetTransform()->CalculateGlobalViewMatrix();

	CreateRectQuad();
	Resize({ 1,1 });
	edited = false;

	percentage_size = 0.035;

	rel_size = float2(1, 1);
}

ComponentRectTransform::~ComponentRectTransform()
{
	delete transform_part;
}

bool ComponentRectTransform::Start()
{
	return true;
}

bool ComponentRectTransform::Update()
{
	if (gameobject->GetComponent(CMP_CANVAS))
		return true;

	GameObject* parent_canvas = GetFirstCanvasParent();

	ComponentCanvasScaler* canvas = (ComponentCanvasScaler*)parent_canvas->GetComponent(CMP_CANVASSCALER);

	if (canvas->GetScaleType() == ST_SCREEN_SIZE && App->imgui->game_panel->size_changed_last_frame)
	{
		float2 new_size = GetSizeFromCanvasPercentage(percentage_size);
		Resize(new_size);
	}

	UpdateRectWithAnchors();

	return true;
}

bool ComponentRectTransform::CleanUp()
{
	return true;
}

void ComponentRectTransform::Draw(bool is_editor)
{
	if (is_editor)

	{
		DrawRectFrame();

		ComponentRectTransform* parent_rect = nullptr;

		if (gameobject->parent)
		{
			GameObject* parent_canvas = GetFirstCanvasParent();

			
			parent_rect = (ComponentRectTransform*)parent_canvas->GetComponent(CMP_RECTTRANSFORM);;

			float3 parent_pos = parent_rect->GetTransform()->GetPosition();

			float3 start_pos = {
				parent_pos.x - parent_rect->width / 2,
				parent_pos.y - parent_rect->height / 2,
				parent_pos.z
			};

			if (gameobject->selected) {
				float3 anchor_min_x_pos, anchor_min_y_pos;

				anchor_min_x_pos = start_pos;
				anchor_min_x_pos.x += parent_rect->width * anchor.min_x;

				DrawAnchorPoint(anchor_min_x_pos, { 10,10 });

				anchor_min_y_pos = start_pos;
				anchor_min_y_pos.y += parent_rect->height * anchor.min_y;

				DrawAnchorPoint(anchor_min_y_pos, { -10,-10 });
			}
		}

		DebugDrawRectSize();


	}
}

void ComponentRectTransform::DrawAnchorPoint(float3 pos, float2 lines_lenght)
{
	glLineWidth(5.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x - lines_lenght.x, pos.y, pos.z);

	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x, pos.y + lines_lenght.y, pos.z);

	glEnd();
}

void ComponentRectTransform::OnEvent(const Event & new_event)
{
}

float3 ComponentRectTransform::GetGlobalPosition()
{
	ComponentRectTransform* curr_cmp = this;
	float3 ret_value = { 0,0,0 };

	while (curr_cmp != nullptr)
	{
		ret_value += curr_cmp->GetTransform()->GetPosition();

		if (curr_cmp->GetGameObject()->GetParent() == nullptr)
			break;

		curr_cmp = (ComponentRectTransform*)curr_cmp->gameobject->GetParent()->GetComponent(CMP_RECTTRANSFORM);
	}

	return ret_value;
}

void ComponentRectTransform::AddaptRectToScreenSize()
{
	float2 screen_tex_size = float2(App->imgui->game_panel->GetGameTexSize().x, App->imgui->game_panel->GetGameTexSize().y);
	Resize(screen_tex_size);
}

void ComponentRectTransform::CreateRectQuad()
{
	quad_mesh = new ComponentMesh(nullptr);

	Mesh* square = new Mesh();
	square->SetVertPlaneData();
	square->LoadToMemory();

	quad_mesh->SetMesh(square);
	quad_mesh->CreateEnclosedMeshAABB();
	quad_mesh->UpdateBoundingBox(GetTransform());
}

void ComponentRectTransform::DrawRectFrame()
{
	App->renderer3D->UseDebugRenderSettings();

	float4x4 view_mat = float4x4::identity;

	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	view_mat.Set((float*)matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)((GetTransform()->GetGlobalViewMatrix()).Transposed() * view_mat).v);

	DebugDrawPlane(quad_mesh->GetMesh()->vertices, Color(1.0f, 1.0f, 1.0f));
	App->renderer3D->GetDefaultRenderSettings();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)view_mat.v);
}

void ComponentRectTransform::DebugDrawRectSize()
{
	float3 real_pos = GetTransform()->GetPosition();

	ComponentRectTransform* parent_rect = nullptr;

	if (gameobject->parent)
	{
		parent_rect = gameobject->parent->rect_transform;
		real_pos.x += parent_rect->width / 2;
		real_pos.y += parent_rect->height / 2;
	}
}

void ComponentRectTransform::Resize(float2 new_size)
{
	float2 half_size = new_size / 2;
	float2 prev_size = { width, height }; 

	quad_mesh->GetMesh()->vertices[0] = { -half_size.x, half_size.y, 0 };
	quad_mesh->GetMesh()->vertices[1] = { half_size.x, half_size.y, 0 };
	quad_mesh->GetMesh()->vertices[2] = { -half_size.x, -half_size.y, 0 };
	quad_mesh->GetMesh()->vertices[3] = { half_size.x, -half_size.y, 0 };

	width = new_size.x;
	height = new_size.y;

	// Set a proper canvas position
	if (gameobject->GetComponent(CMP_CANVAS) != nullptr)
		GetTransform()->SetPosition({ half_size.x, half_size.y, 0 });

	// Addapt plane components if needed
	for (auto it = gameobject->component_list.begin(); it != gameobject->component_list.end(); it++)
	{
		(*it)->FitToRect();

		/*if ((*it)->GetType() == CMP_TEXT)
		{
			ComponentText* cmp_txt = (ComponentText*)(*it);
			cmp_txt->GetLabel()->text_size = GetFontSizeFromRectSize(); 
			cmp_txt->GetLabel()->ResizeFont(); 
		}*/
	}
		

}

float2 ComponentRectTransform::GetSizeFromPercentage(float value, UI_Widgget_Type type)
{
	float2 ret_size;

	ret_size.x = width * value;
	ret_size.y = width * value;

	if (type == UI_Widgget_Type::UI_LABEL || type == UI_Widgget_Type::UI_INPUTFIELD)
		ret_size.y = ret_size.x * 0.25f;

	return ret_size;
}

float2 ComponentRectTransform::GetSizeFromCanvasPercentage(const float& percentage)
{
	float2 ret_size;

	GameObject* parent_canvas = GetFirstCanvasParent();

	ComponentRectTransform* canvas = (ComponentRectTransform*)parent_canvas->GetComponent(CMP_RECTTRANSFORM);

	ret_size.x = canvas->width * percentage * rel_size.x;
	ret_size.y = canvas->width * percentage * rel_size.y;

	return ret_size;
}

float3 ComponentRectTransform::GetPointFromPercentage(const float2& percentage)
{
	float point_x =  GetSize().x * percentage.x;
	float point_y =  GetSize().y * percentage.y;

	return { point_x, point_y, 0 };
}

float3 ComponentRectTransform::GetRectCenter()
{
	float3 return_value = { GetSize().x / 2.0f, GetSize().y / 2.0f,  0 }; 
	return_value += GetGlobalPosition();

	return return_value;
}

int ComponentRectTransform::GetFontSizeFromRectSize()
{
	int font_size = width / CANVAS_FONT_REL;
	return font_size;
}

void ComponentRectTransform::CompensateParentRelativePos()
{

}

float2 ComponentRectTransform::GetRelativePos() const
{
	return relative_pos;
}

void ComponentRectTransform::SetRelativePos(float2 new_pos)
{
	relative_pos = new_pos;
	quad_mesh->UpdateBoundingBox(GetTransform());
}

void ComponentRectTransform::TranslateRelativePos(float2 new_pos)
{
	relative_pos += new_pos;
	quad_mesh->UpdateBoundingBox(GetTransform());
}

Mesh * ComponentRectTransform::GetRectQuad() const
{
	return quad_mesh->GetMesh();
}

ComponentMesh * ComponentRectTransform::GetRectQuadComponent() const
{
	return quad_mesh;
}

void ComponentRectTransform::UpdateRectWithAnchors()
{
	float3 real_pos = transform_part->GetPosition();
	float3 parent_pos = { 0,0,0 };
	ComponentRectTransform* parent_rect = nullptr;
	ComponentTransform* parent_transform = nullptr;

	GameObject* parent_canvas = GetFirstCanvasParent();

	ComponentCanvasScaler* canvas = (ComponentCanvasScaler*)parent_canvas->GetComponent(CMP_CANVASSCALER);

	if (gameobject->parent)
		parent_rect = gameobject->parent->rect_transform;

	if (parent_rect)
	{
		parent_transform = parent_rect->transform_part;

		parent_pos = parent_transform->GetPosition();

		float2 start_pos = { 0,0 };

		if (gameobject->parent->GetComponent(CMP_CANVAS)) { //as canvas is never centered
			start_pos = {
			parent_pos.x - parent_rect->width,
			parent_pos.y - parent_rect->height
			};
		}
		else {
			start_pos = {
			parent_pos.x - parent_rect->width / 2,
			parent_pos.y - parent_rect->height / 2
			};
		}

		if (canvas->GetScaleType() == ST_CONSTANT)
		{
			real_pos.y = (start_pos.y + (anchor.min_y * parent_rect->height) + relative_pos.y);
			real_pos.x = (start_pos.x + (anchor.min_x * parent_rect->width) + relative_pos.x);
		}
		else
		{
			real_pos.x = (start_pos.x + (anchor.min_x * parent_rect->width) + (relative_pos.x * width * canvas->relative_pos_scaler));
			real_pos.y = (start_pos.y + (anchor.min_y * parent_rect->height) + (relative_pos.y * height * canvas->relative_pos_scaler));
		}


		//to prevent positions accumulating
		if (!gameobject->parent->GetComponent(CMP_CANVAS)) {
			if (gameobject->parent->GetComponent(CMP_RECTTRANSFORM)) //parent atribute is nullptr?
			{
				ComponentRectTransform* parent_rect_trans = (ComponentRectTransform*)gameobject->parent->GetComponent(CMP_RECTTRANSFORM);
				ComponentTransform* parent_trans = parent_rect_trans->transform_part;


				float2 offset = { 0,0 };
				offset.x = -parent_trans->GetPosition().x;
				offset.y = -parent_trans->GetPosition().y;
				real_pos.y += offset.y;
				real_pos.x += offset.x;

			}
		}

		//if (gameobject->GetComponent(CMP_TEXT))
		//{
		//	real_pos.x -= width;
		//}

		transform_part->SetPosition(real_pos);
	}

}

bool ComponentRectTransform::GetClosestIntersectionPoint(LineSegment line, float3 & closest_point, float & distance)
{
	AABB ui_aabb = App->user_interface->GetRenderBox();
	ComponentTransform* trans = GetTransform();
	Triangle tri;
	float current_distance;
	float closest_distance = 10000; //Hmm maybe there is a better way to do this
	float3 hit_point;
	bool ret = false;

	if (!quad_mesh)
		return false;

	if (!quad_mesh->GetMesh()->vertices)
		return false;

	int num_tris = quad_mesh->GetMesh()->num_indices / 3;

	float4x4 gm = trans->GetGlobalViewMatrix();

	line.Transform(gm.Inverted());

	for (int i = 0; i < quad_mesh->GetMesh()->num_indices; i += 3)
	{
		float3 vertex_a = quad_mesh->GetMesh()->vertices[quad_mesh->GetMesh()->indices[i]];
		float3 vertex_b = quad_mesh->GetMesh()->vertices[quad_mesh->GetMesh()->indices[i + 1]];
		float3 vertex_c = quad_mesh->GetMesh()->vertices[quad_mesh->GetMesh()->indices[i + 2]];

		tri.a = vertex_a;
		tri.b = vertex_b;
		tri.c = vertex_c;

		bool hit = line.Intersects(tri, &current_distance, &hit_point);

		if (hit)
		{
			//CONSOLE_LOG("hitpoint:  x:%f, y:%f, z:%f", hit_point.x, hit_point.y, hit_point.z);
			if (current_distance < closest_distance)
			{
				closest_point = hit_point;
				closest_distance = current_distance;
				ret = true;
			}
		}
	}

	closest_point += gm.TranslatePart();
	distance = closest_distance;

	//if (ret)
	//	CONSOLE_LOG("x:%f, y:%f, z:%f distance:%f", closest_point.x, closest_point.y, closest_point.z, closest_distance);
	return ret;
}

bool ComponentRectTransform::GetClosestIntersectionPointForGame(LineSegment line, float3 & closest_point, float & distance)
{
	AABB ui_aabb = App->user_interface->GetRenderBox();
	ComponentTransform* trans = GetTransform();
	Triangle tri;
	float current_distance;
	float closest_distance = 10000; //Hmm maybe there is a better way to do this
	float3 hit_point;
	bool ret = false;

	if (!quad_mesh)
		return false;

	if (!quad_mesh->GetMesh()->vertices)
		return false;

	int num_tris = quad_mesh->GetMesh()->num_indices / 3;

	float4x4 gm = trans->GetGlobalViewMatrix();

	//Find the canvas


	line.a.x += 750 / 2;
	line.b.x += 750 / 2;
	line.a.y += 422 / 2;
	line.b.y += 422 / 2;

	line.Transform(gm.Inverted());

	for (int i = 0; i < quad_mesh->GetMesh()->num_indices; i += 3)
	{
		float3 vertex_a = quad_mesh->GetMesh()->vertices[quad_mesh->GetMesh()->indices[i]];
		float3 vertex_b = quad_mesh->GetMesh()->vertices[quad_mesh->GetMesh()->indices[i + 1]];
		float3 vertex_c = quad_mesh->GetMesh()->vertices[quad_mesh->GetMesh()->indices[i + 2]];

		tri.a = vertex_a;
		tri.b = vertex_b;
		tri.c = vertex_c;

		bool hit = line.Intersects(tri, &current_distance, &hit_point);

		if (hit)
		{
			//CONSOLE_LOG("hitpoint:  x:%f, y:%f, z:%f", hit_point.x, hit_point.y, hit_point.z);
			if (current_distance < closest_distance)
			{
				closest_point = hit_point;
				closest_distance = current_distance;
				ret = true;
			}
		}
	}

	closest_point += gm.TranslatePart();
	distance = closest_distance;

	//if (ret)
	//	CONSOLE_LOG("x:%f, y:%f, z:%f distance:%f", closest_point.x, closest_point.y, closest_point.z, closest_distance);
	return ret;
}

GameObject * ComponentRectTransform::GetFirstCanvasParent()
{
	GameObject* parent_canvas = gameobject;

	while (!parent_canvas->GetComponent(CMP_CANVAS)) {
		parent_canvas = parent_canvas->parent;
	}

	return parent_canvas;
}

void ComponentRectTransform::FitToParentRect()
{
	float2 parent_size = parent->GetSize();
	Resize(parent_size);
}

float2 ComponentRectTransform::GetSize() const
{
	return { width, height };
}

void ComponentRectTransform::ResizeWithPercentage()
{
}

void ComponentRectTransform::Load(JSON_Object * json_obj)
{
	// Get the mesh --------	
	std::string resource_name = json_object_dotget_string(json_obj, "ComponentMesh.MeshName");	//As it's UI, it will always be the plane

	Mesh* plane_mesh = new Mesh();
	plane_mesh->SetVertPlaneData();
	plane_mesh->LoadToMemory();

	quad_mesh->SetMesh(plane_mesh);

	float2 size = { (float)json_object_dotget_number(json_obj, "Width"),  (float)json_object_dotget_number(json_obj, "Height") };
	Resize(size);

	// Apply the transform ------
	float3 pos = float3::zero;
	float3 rot = float3::zero;
	float3 scale = float3::zero;
	float2 tmp_relative_pos = float2::zero;

	pos.x = json_object_dotget_number(json_obj, "ComponentTransform.PositionX");
	pos.y = json_object_dotget_number(json_obj, "ComponentTransform.PositionY");
	pos.z = json_object_dotget_number(json_obj, "ComponentTransform.PositionZ");

	rot.x = json_object_dotget_number(json_obj, "ComponentTransform.RotationX");
	rot.y = json_object_dotget_number(json_obj, "ComponentTransform.RotationY");
	rot.z = json_object_dotget_number(json_obj, "ComponentTransform.RotationZ");

	scale.x = json_object_dotget_number(json_obj, "ComponentTransform.ScaleX");
	scale.y = json_object_dotget_number(json_obj, "ComponentTransform.ScaleY");
	scale.z = json_object_dotget_number(json_obj, "ComponentTransform.ScaleZ");

	tmp_relative_pos.x = json_object_dotget_number(json_obj, "RelativePos.X");
	tmp_relative_pos.y = json_object_dotget_number(json_obj, "RelativePos.Y");

	transform_part->SetPosition(pos);
	transform_part->SetRotationEuler(rot);
	transform_part->SetScale(scale);

	SetRelativePos(tmp_relative_pos);
}

void ComponentRectTransform::Save(JSON_Object * json_obj, const char * root)
{
	std::string node_name = root + std::string(".ComponentRectTransform");
	std::string item_name = "";

	//We need to save the transform, the quad mesh and the parent. 
	// Transform
	GetTransform()->Save(json_obj, node_name.c_str());

	// Quad Mesh
	GetRectQuadComponent()->Save(json_obj, node_name.c_str());

	std::string width_name = node_name + ".Width";
	std::string height_name = node_name + ".Height";

	json_object_dotset_number(json_obj, width_name.c_str(), width);
	json_object_dotset_number(json_obj, height_name.c_str(), height);

	std::string rel_x = node_name + ".RelativePos.X";
	std::string rel_y = node_name + ".RelativePos.Y";

	json_object_dotset_number(json_obj, rel_x.c_str(), GetRelativePos().x);
	json_object_dotset_number(json_obj, rel_y.c_str(), GetRelativePos().y);
}


ComponentTransform* ComponentRectTransform::GetTransform()
{
	return transform_part;
}

void ComponentRectTransform::SetAnchorPoint(float min_x, float min_y, float max_x, float max_y)
{
	ComponentRectTransform* parent_rect = nullptr;

	if (gameobject->parent)
		parent_rect = gameobject->parent->rect_transform;

	if (!parent_rect)
		return;

	float x_diff = (min_x - anchor.min_x) * parent_rect->width;
	float y_diff = (min_y - anchor.min_y) * parent_rect->height;

	relative_pos.x -= x_diff;
	relative_pos.y -= y_diff;

	anchor.min_x = min_x;
	anchor.min_y = min_y;
	anchor.max_x = max_x;
	anchor.max_y = max_y;
}

AnchorPoints ComponentRectTransform::GetAnchorPoint() const
{
	return anchor;
}
