#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
//#include "build/Curves/Bezier1D.h"
//#include "build/Curves/Bezier2D.h"


class MeshConstructor
{
	VertexArray vao;
	IndexBuffer* ib;
	//TO DO: add bounding box data base and build it in the constructor 
	std::vector<VertexBuffer*> vbs;
	bool is2D;
	int unsigned indicesNum;

	void InitLine(const IndexedModel& model);
	void InitMesh(const IndexedModel& model);
	void CopyMesh(const MeshConstructor& mesh);
	void CopyLine(const MeshConstructor& mesh);

	static const unsigned int VEC3_ATTRIB_NUM = 3;
	static const unsigned int VEC2_ATTRIB_NUM = 1;

public:
	//TO DO: add collision detection function which get other MeshConstructor and Mat4 of related transformasions. The function may return a pointer to the relevant Bounding Box when collide
	void ChangeLine(IndexedModel& model);
	enum SimpleShapes
	{
		Axis,
		Plane,
		Cube,
		Octahedron,
		Tethrahedron,
	};
	MeshConstructor(const int type);
	MeshConstructor(const MeshConstructor& mesh);
	MeshConstructor(const std::string& fileName);
	MeshConstructor(const IndexedModel& model, bool isMesh);
	//MeshConstructor::MeshConstructor(Bezier2D* surf);

	void Bind() { vao.Bind(); }
	void Unbind() { vao.Unbind(); }
	inline unsigned int GetIndicesNum() { return indicesNum; }
	inline bool Is2D() { return is2D; }
	~MeshConstructor(void);

};

