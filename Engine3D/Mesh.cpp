#include "Mesh.h"

void IndexedModel::CalcNormals()
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = positions[i1] - positions[i0];
		glm::vec3 v2 = positions[i2] - positions[i0];

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (unsigned int i = 0; i < positions.size(); i++)
	{
		normals[i] = glm::normalize(normals[i]);
		colors[i] = (glm::vec3(1, 1, 1) + normals[i]) / 2.0f;
	}
}

IndexedModel PlaneTriangles()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	IndexedModel model;

	for (unsigned int i = 0; i < 4; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for (unsigned int i = 0; i < 6; i++)
		model.indices.push_back(indices[i]);

	return model;
}

IndexedModel CubeTriangles()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 1), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1))
	};


	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};

	IndexedModel model;

	for (unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for (unsigned int i = 0; i < 36; i++)
		model.indices.push_back(indices[i]);

	return model;
}

IndexedModel AxisGenerator()
{
	IndexedModel model;

	LineVertex axisVertices[] =
	{
		LineVertex(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)),
		LineVertex(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),
		LineVertex(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
		LineVertex(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),
		LineVertex(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)),
		LineVertex(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)),
	};


	unsigned int axisIndices[] =
	{
		0, 1,
		2, 3,
		4, 5
	};

	for (unsigned int i = 0; i < 6; i++)
	{
		model.positions.push_back(*axisVertices[i].GetPos());
		model.colors.push_back(*axisVertices[i].GetColor());
	}
	for (unsigned int i = 0; i < 6; i++)
		model.indices.push_back(axisIndices[i]);

	return model;
}

IndexedModel TethrahedronGenerator()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(1, -1, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(1, -1, 1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 1), glm::vec3(1, -1, 1), glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(1, 1, -1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(1, 1, -1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(1, 1, -1), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 0)),

	};


	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11

	};

	IndexedModel model;

	for (unsigned int i = 0; i < 12; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for (unsigned int i = 0; i < 12; i++)
		model.indices.push_back(indices[i]);

	return model;
}

IndexedModel OctahedronGenerator()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(1, 1, -1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(1, 1, -1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(1, 1, -1), glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(1, -1, -1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(0, 0), glm::vec3(1, -1, -1), glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(1, -1, -1), glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(-1, -1, -1), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 1), glm::vec3(-1, -1, -1), glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(-1, -1, -1), glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(-1, -1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(-1, -1, 1), glm::vec3(1, 0, 0)),


		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(1, -1, 1), glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(1, -1, 1), glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(1, -1, 1), glm::vec3(1, 0, 1)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(-1, 1, 1), glm::vec3(0.7f, 0.7f, 0.7f)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 0), glm::vec3(-1, 1, 1), glm::vec3(0.7f, 0.7f, 0.7f)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(-1, 1, 1), glm::vec3(0.7f, 0.7f, 0.7f)),

	};


	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
	};

	IndexedModel model;

	for (unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for (unsigned int i = 0; i < 24; i++)
		model.indices.push_back(indices[i]);

	return model;
}
