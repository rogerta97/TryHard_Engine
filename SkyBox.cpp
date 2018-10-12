#include "SkyBox.h"
#include "Application.h"

//int num_uvs = num_indices;
//float* uvs_cords = new float[num_uvs * 2];

//uvs_cords[0] = 0.0f;
//uvs_cords[1] = 1.0f;

//uvs_cords[2] = 0.0f;
//uvs_cords[3] = 0.0f;

//uvs_cords[4] = 1.0f;
//uvs_cords[5] = 1.0f;

//uvs_cords[6] = 1.0f;
//uvs_cords[7] = 0.0f;

//uvs_cords[8] = 1.0f;
//uvs_cords[9] = 1.0f;

//uvs_cords[10] = 0.0f;
//uvs_cords[11] = 0.0f;	

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::InitSkyBox(const char * folder_name)
{
	CreateFrontPlane(); 
	CreateRightPlane();
	CreateLeftPlane();
	CreateBackPlane();
	CreateTopPlane();
	CreateDownPlane();	
}

void SkyBox::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	for (int i = 0; i < 6; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, sky_cube[i].vertices_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[i].indices_id);
		glDrawElements(GL_TRIANGLES, sky_cube[i].num_indices, GL_UNSIGNED_INT, NULL);	
	}

	glDisableClientState(GL_VERTEX_ARRAY);

}

void SkyBox::CreateFrontPlane()
{
	//Create Front Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_FRONT] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_FRONT].num_vertices = 4;
	sky_cube[SKYBOX_FRONT].vertices = new float3[4];

	sky_cube[SKYBOX_FRONT].vertices[0].x = 100.0f;
	sky_cube[SKYBOX_FRONT].vertices[0].y = 100.0f;
	sky_cube[SKYBOX_FRONT].vertices[0].z = 100.0f;

	sky_cube[SKYBOX_FRONT].vertices[1].x = -100.0;
	sky_cube[SKYBOX_FRONT].vertices[1].y = 100.0f;
	sky_cube[SKYBOX_FRONT].vertices[1].z = 100.0f;

	sky_cube[SKYBOX_FRONT].vertices[2].x = 100.0f;
	sky_cube[SKYBOX_FRONT].vertices[2].y = -100.0f;
	sky_cube[SKYBOX_FRONT].vertices[2].z = 100.0f;

	sky_cube[SKYBOX_FRONT].vertices[3].x = -100.0;
	sky_cube[SKYBOX_FRONT].vertices[3].y = -100.0f;
	sky_cube[SKYBOX_FRONT].vertices[3].z = 100.0f;

	// ---------------------

	sky_cube[SKYBOX_FRONT].vertices_id = sky_cube[SKYBOX_FRONT].CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_FRONT].vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_FRONT].vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_FRONT].num_indices = 6;
	sky_cube[SKYBOX_FRONT].indices = new int[6];

	sky_cube[SKYBOX_FRONT].indices[0] = 2;
	sky_cube[SKYBOX_FRONT].indices[1] = 1;
	sky_cube[SKYBOX_FRONT].indices[2] = 0;
	sky_cube[SKYBOX_FRONT].indices[3] = 2;
	sky_cube[SKYBOX_FRONT].indices[4] = 3;
	sky_cube[SKYBOX_FRONT].indices[5] = 1;

	// ---------------------

	sky_cube[SKYBOX_FRONT].indices_id = sky_cube[SKYBOX_FRONT].CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_FRONT].indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_FRONT].indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// ----------------------------------------------------------------------------
}

void SkyBox::CreateRightPlane()
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_RIGHT] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_RIGHT].num_vertices = 4;
	sky_cube[SKYBOX_RIGHT].vertices = new float3[4];

	sky_cube[SKYBOX_RIGHT].vertices[0].x = -100.0f;
	sky_cube[SKYBOX_RIGHT].vertices[0].y = 100.0f;
	sky_cube[SKYBOX_RIGHT].vertices[0].z = 100.0f;

	sky_cube[SKYBOX_RIGHT].vertices[1].x = -100.0;
	sky_cube[SKYBOX_RIGHT].vertices[1].y = 100.0f;
	sky_cube[SKYBOX_RIGHT].vertices[1].z = -100.0f;

	sky_cube[SKYBOX_RIGHT].vertices[2].x = -100.0f;
	sky_cube[SKYBOX_RIGHT].vertices[2].y = -100.0f;
	sky_cube[SKYBOX_RIGHT].vertices[2].z = 100.0f;

	sky_cube[SKYBOX_RIGHT].vertices[3].x = -100.0;
	sky_cube[SKYBOX_RIGHT].vertices[3].y = -100.0f;
	sky_cube[SKYBOX_RIGHT].vertices[3].z = -100.0f;

	// ---------------------

	sky_cube[SKYBOX_RIGHT].vertices_id = sky_cube[SKYBOX_RIGHT].CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_RIGHT].vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_RIGHT].vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_RIGHT].num_indices = 6;
	sky_cube[SKYBOX_RIGHT].indices = new int[6];

	sky_cube[SKYBOX_RIGHT].indices[0] = 1;
	sky_cube[SKYBOX_RIGHT].indices[1] = 0;
	sky_cube[SKYBOX_RIGHT].indices[2] = 2;
	sky_cube[SKYBOX_RIGHT].indices[3] = 1;
	sky_cube[SKYBOX_RIGHT].indices[4] = 2;
	sky_cube[SKYBOX_RIGHT].indices[5] = 3;

	// ---------------------

	sky_cube[SKYBOX_RIGHT].indices_id = sky_cube[SKYBOX_RIGHT].CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_RIGHT].indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_RIGHT].indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------
}

void SkyBox::CreateLeftPlane()
{
	//Create Left Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_LEFT] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_LEFT].num_vertices = 4;
	sky_cube[SKYBOX_LEFT].vertices = new float3[4];

	sky_cube[SKYBOX_LEFT].vertices[0].x = 100.0f;
	sky_cube[SKYBOX_LEFT].vertices[0].y = 100.0f;
	sky_cube[SKYBOX_LEFT].vertices[0].z = 100.0f;

	sky_cube[SKYBOX_LEFT].vertices[1].x = 100.0;
	sky_cube[SKYBOX_LEFT].vertices[1].y = 100.0f;
	sky_cube[SKYBOX_LEFT].vertices[1].z = -100.0f;

	sky_cube[SKYBOX_LEFT].vertices[2].x = 100.0f;
	sky_cube[SKYBOX_LEFT].vertices[2].y = -100.0f;
	sky_cube[SKYBOX_LEFT].vertices[2].z = 100.0f;
			
	sky_cube[SKYBOX_LEFT].vertices[3].x = 100.0;
	sky_cube[SKYBOX_LEFT].vertices[3].y = -100.0f;
	sky_cube[SKYBOX_LEFT].vertices[3].z = -100.0f;

	// ---------------------

	sky_cube[SKYBOX_LEFT].vertices_id = sky_cube[SKYBOX_LEFT].CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_LEFT].vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_LEFT].vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_LEFT].num_indices = 6;
	sky_cube[SKYBOX_LEFT].indices = new int[6];

	sky_cube[SKYBOX_LEFT].indices[0] = 0;
	sky_cube[SKYBOX_LEFT].indices[1] = 1;
	sky_cube[SKYBOX_LEFT].indices[2] = 2;
	sky_cube[SKYBOX_LEFT].indices[3] = 2;
	sky_cube[SKYBOX_LEFT].indices[4] = 1;
	sky_cube[SKYBOX_LEFT].indices[5] = 3;

	// ---------------------

	sky_cube[SKYBOX_LEFT].indices_id = sky_cube[SKYBOX_LEFT].CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_LEFT].indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_LEFT].indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------
}

void SkyBox::CreateBackPlane()
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_BACK] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_BACK].num_vertices = 4;
	sky_cube[SKYBOX_BACK].vertices = new float3[4];
			
	sky_cube[SKYBOX_BACK].vertices[0].x = 100.0f;
	sky_cube[SKYBOX_BACK].vertices[0].y = 100.0f;
	sky_cube[SKYBOX_BACK].vertices[0].z = -100.0f;
		
	sky_cube[SKYBOX_BACK].vertices[1].x = -100.0;
	sky_cube[SKYBOX_BACK].vertices[1].y = 100.0f;
	sky_cube[SKYBOX_BACK].vertices[1].z = -100.0f;
			
	sky_cube[SKYBOX_BACK].vertices[2].x = 100.0f;
	sky_cube[SKYBOX_BACK].vertices[2].y = -100.0f;
	sky_cube[SKYBOX_BACK].vertices[2].z = -100.0f;
			
	sky_cube[SKYBOX_BACK].vertices[3].x = -100.0;
	sky_cube[SKYBOX_BACK].vertices[3].y = -100.0f;
	sky_cube[SKYBOX_BACK].vertices[3].z = -100.0f;

	// ---------------------

	sky_cube[SKYBOX_BACK].vertices_id = sky_cube[SKYBOX_BACK].CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_BACK].vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_BACK].vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_BACK].num_indices = 6;
	sky_cube[SKYBOX_BACK].indices = new int[6];
			
	sky_cube[SKYBOX_BACK].indices[0] = 0;
	sky_cube[SKYBOX_BACK].indices[1] = 1;
	sky_cube[SKYBOX_BACK].indices[2] = 2;
	sky_cube[SKYBOX_BACK].indices[3] = 2;
	sky_cube[SKYBOX_BACK].indices[4] = 1;
	sky_cube[SKYBOX_BACK].indices[5] = 3;

	// ---------------------

	sky_cube[SKYBOX_BACK].indices_id = sky_cube[SKYBOX_BACK].CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_BACK].indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_BACK].indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------
}

void SkyBox::CreateTopPlane()
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_TOP] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_TOP].num_vertices = 4;
	sky_cube[SKYBOX_TOP].vertices = new float3[4];
			 
	sky_cube[SKYBOX_TOP].vertices[0].x = 100.0f;
	sky_cube[SKYBOX_TOP].vertices[0].y = 100.0f;
	sky_cube[SKYBOX_TOP].vertices[0].z = 100.0f;
			 
	sky_cube[SKYBOX_TOP].vertices[1].x = -100.0;
	sky_cube[SKYBOX_TOP].vertices[1].y = 100.0f;
	sky_cube[SKYBOX_TOP].vertices[1].z = 100.0f;
			 
	sky_cube[SKYBOX_TOP].vertices[2].x = 100.0f;
	sky_cube[SKYBOX_TOP].vertices[2].y = 100.0f;
	sky_cube[SKYBOX_TOP].vertices[2].z = -100.0f;
			 
	sky_cube[SKYBOX_TOP].vertices[3].x = -100.0;
	sky_cube[SKYBOX_TOP].vertices[3].y = 100.0f;
	sky_cube[SKYBOX_TOP].vertices[3].z = -100.0f;

	// ---------------------

	sky_cube[SKYBOX_TOP].vertices_id = sky_cube[SKYBOX_TOP].CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_TOP].vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_TOP].vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_TOP].num_indices = 6;
	sky_cube[SKYBOX_TOP].indices = new int[6];
			 
	sky_cube[SKYBOX_TOP].indices[0] = 2;
	sky_cube[SKYBOX_TOP].indices[1] = 0;
	sky_cube[SKYBOX_TOP].indices[2] = 1;
	sky_cube[SKYBOX_TOP].indices[3] = 3;
	sky_cube[SKYBOX_TOP].indices[4] = 2;
	sky_cube[SKYBOX_TOP].indices[5] = 1;

	// ---------------------

	sky_cube[SKYBOX_TOP].indices_id = sky_cube[SKYBOX_TOP].CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_TOP].indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_TOP].indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------
}

void SkyBox::CreateDownPlane()
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_DOWN] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_DOWN].num_vertices = 4;
	sky_cube[SKYBOX_DOWN].vertices = new float3[4];

	sky_cube[SKYBOX_DOWN].vertices[0].x = 100.0f;
	sky_cube[SKYBOX_DOWN].vertices[0].y = -100.0f;
	sky_cube[SKYBOX_DOWN].vertices[0].z = 100.0f;
			 
	sky_cube[SKYBOX_DOWN].vertices[1].x = -100.0;
	sky_cube[SKYBOX_DOWN].vertices[1].y = -100.0f;
	sky_cube[SKYBOX_DOWN].vertices[1].z = 100.0f;
			 
	sky_cube[SKYBOX_DOWN].vertices[2].x = 100.0f;
	sky_cube[SKYBOX_DOWN].vertices[2].y = -100.0f;
	sky_cube[SKYBOX_DOWN].vertices[2].z = -100.0f;
			 
	sky_cube[SKYBOX_DOWN].vertices[3].x = -100.0;
	sky_cube[SKYBOX_DOWN].vertices[3].y = -100.0f;
	sky_cube[SKYBOX_DOWN].vertices[3].z = -100.0f;

	// ---------------------

	sky_cube[SKYBOX_DOWN].vertices_id = sky_cube[SKYBOX_DOWN].CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_DOWN].vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_DOWN].vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_DOWN].num_indices = 6;
	sky_cube[SKYBOX_DOWN].indices = new int[6];
			 
	sky_cube[SKYBOX_DOWN].indices[0] = 2;
	sky_cube[SKYBOX_DOWN].indices[1] = 1;
	sky_cube[SKYBOX_DOWN].indices[2] = 0;
	sky_cube[SKYBOX_DOWN].indices[3] = 2;
	sky_cube[SKYBOX_DOWN].indices[4] = 3;
	sky_cube[SKYBOX_DOWN].indices[5] = 1;

	// ---------------------

	sky_cube[SKYBOX_DOWN].indices_id = sky_cube[SKYBOX_DOWN].CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_DOWN].indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_DOWN].indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------
}
