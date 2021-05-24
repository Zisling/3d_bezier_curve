//#define GLEW_STATIC
//#include <GL\glew.h>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "obj_loader.h"

#define GL_FLOAT 0x1406

//MeshConstructor::MeshConstructor( Bezier1D* c)
//{
//	InitLine(c->GetLine());
//}
//
//MeshConstructor::MeshConstructor(Bezier2D* surf)
//{
//	InitMesh(surf->GetSurface());
//}

MeshConstructor::MeshConstructor(const int type)
{
	switch (type)
	{
	case Axis:
		InitLine(AxisGenerator());
		break;
	case Plane:
		InitMesh(PlaneTriangles());
		break;
	case Cube:
		InitMesh(CubeTriangles());
		break;
	case Octahedron:
		InitMesh(OctahedronGenerator());
		break;
	case Tethrahedron:
		InitMesh(TethrahedronGenerator());
		break;
	default:
		break;
	}

}

MeshConstructor::MeshConstructor(const std::string& fileName)
{
	InitMesh(OBJModel(fileName).ToIndexedModel());
}

MeshConstructor::MeshConstructor(const IndexedModel& model, bool isMesh)
{
	if (isMesh)
		InitMesh(model);
	else
		InitLine(model);
}


MeshConstructor::MeshConstructor(const MeshConstructor& mesh)
{
	indicesNum = mesh.indicesNum;
	if (mesh.is2D)
		CopyMesh(mesh);
	else
		CopyLine(mesh);
}

MeshConstructor::~MeshConstructor(void)
{
	if (ib)
		delete ib;
	for (unsigned int i = 0; i < vbs.size(); i++)
	{
		if (vbs[i])
			delete vbs[i];
	}
}

void MeshConstructor::InitLine(const IndexedModel& model) {

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();

	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum * sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	ib = new IndexBuffer((unsigned int*)model.GetData(VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM), indicesNum);

	vao.Unbind();
	is2D = false;
}

void MeshConstructor::InitMesh(const IndexedModel& model) {

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();

	vao.Bind();
	int i = 0;
	for (; i < VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum * sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}
	for (; i < VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum * sizeof(model.texCoords[0])));
		vao.AddBuffer(*vbs.back(), i, 2, GL_FLOAT);
	}

	ib = new IndexBuffer((unsigned int*)model.GetData(i), indicesNum);

	vao.Unbind();
	//is2D = true;

}

void MeshConstructor::CopyLine(const MeshConstructor& mesh) {

	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	ib = new IndexBuffer(*mesh.ib);

	vao.Unbind();

	is2D = false;

}

void MeshConstructor::ChangeLine(IndexedModel& model)
{
	int verticesNum = model.positions.size(); // to change only part of the line
	//indicesNum = model.indices.size();

	vao.Bind();

	for (int i = 0; i < 1; i++) //to add a dirty flag
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum * sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	//	ib = new IndexBuffer((unsigned int*)model.GetData(VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM), indicesNum);

	vao.Unbind();

}

void MeshConstructor::CopyMesh(const MeshConstructor& mesh) {

	vao.Bind();
	int i = 0;
	for (; i < VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}
	for (; i < VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 2, GL_FLOAT);
	}

	ib = new IndexBuffer(*mesh.ib);
	//ib = mesh.ib;
	vao.Unbind();

	is2D = true;

}
