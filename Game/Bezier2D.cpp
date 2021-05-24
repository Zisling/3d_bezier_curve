#include "Bezier2D.h"

#include <iostream>
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/_vectorize.hpp>



#include "Bezier1D.h"
#include "Bezier1D.h"
#include "Bezier1D.h"

Bezier2D::Bezier2D(): Shape(-1, QUADS), circularSubdivision(0), resT(0), resS(0), subNum(0)
{
	false;
}

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}
}

static void printVec(const glm::vec4 vec)
{
	std::cout << " vector:" << std::endl;
	for (int j = 0; j < 4; j++)
		std::cout << vec[j] << " ";
	std::cout << std::endl;
}

Bezier2D::Bezier2D(const Bezier1D* c, int circular_subdivision, const int res_s, const int res_t, int mode,
                   int viewport):
	Shape(CubeTriangles(), true, mode, viewport), circularSubdivision(circular_subdivision), resT(res_t), resS(res_s),
	subNum(c->GetSegmentsNum())
{
	CalcControlPoints(c);
	IndexedModel model = GetSurface();
	mesh = new MeshConstructor(model, true);
}

IndexedModel Bezier2D::GetSurface()
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	int index = 0;
	glm::mat4 point_idxs = {
		{0, 4, 8, 12},
		{1, 5, 9, 13},
		{2, 6, 10, 14},
		{3, 7, 11, 15}
	};
	glm::vec3 color(1, 1, 1);
	glm::vec3 normal;
	glm::vec2 texCoord0;
	glm::vec2 texCoord1;
	glm::vec2 texCoord2;
	glm::vec2 texCoord3;
	glm::vec4 vert_pos0;
	glm::vec4 vert_pos1;
	glm::vec4 vert_pos2;
	glm::vec4 vert_pos3;
	for (int t = 0; t < circularSubdivision; t++)
	{
		for (int i = 0; i < subNum; i++)
		{
			for (int j = 0; j < resS; j++)
			{
				for (int k = 0; k < resT; ++k)
				{
					vert_pos0 = CalcPoint(float(j) / resS, float(k) / resT, point_idxs);
					vert_pos1 = CalcPoint(float(j) / resS, float(k + 1) / resT, point_idxs);

					vert_pos2 = CalcPoint(float(j + 1) / resS, float(k + 1) / resT, point_idxs);
					vert_pos3 = CalcPoint(float(j + 1) / resS, float(k) / resT, point_idxs);
					
					normal = GetNormal(float(j) / resS, float(k) / resT, point_idxs);
					if(glm::length(normal) < 0.00000000001)
						normal = GetNormal(float(j + 1) / resS, float(k + 1) / resT, point_idxs);
					if(glm::length(normal) < 0.00000000001)
						normal = GetNormal(float(j) / resS, float(k + 1) / resT, point_idxs);
					if (glm::length(normal) < 0.0000000001)
						normal = GetNormal(float(j+1) / resS, float(k) / resT, point_idxs);

					texCoord0 = glm::vec2(float(k) / float(resT), float(j) / float(resS));
					texCoord1 = glm::vec2(float(k + 1) / float(resT), float(j) / float(resS));
					texCoord2 = glm::vec2(float(k + 1) / float(resT), float(j + 1) / float(resS));
					texCoord3 = glm::vec2(float(k) / float(resT), float(j + 1) / float(resS));
					
					vertices.emplace_back(glm::vec3(vert_pos0), texCoord0, normal, color);
					indices.push_back(index++);
					vertices.emplace_back(glm::vec3(vert_pos1), texCoord1, normal, color);
					indices.push_back(index++);
					vertices.emplace_back(glm::vec3(vert_pos2), texCoord2, normal, color);
					indices.push_back(index++);
					vertices.emplace_back(glm::vec3(vert_pos3), texCoord3, normal, color);
					indices.push_back(index++);
				}
			}
			point_idxs += 12;
		}
		point_idxs += 4;
	}
	std::cout << texCoord3.x << std::endl;
	IndexedModel model;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for (unsigned int i = 0; i < indices.size(); i++)
		model.indices.push_back(indices[i]);
	return model;
}

glm::vec3 Bezier2D::GetNormal(float s, float t, glm::mat4 point_idxs)
{
	int p0_idx;
	int p1_idx;
	int p2_idx;
	int p3_idx;

	glm::mat4 Pi;
	glm::mat4 Pj;
	glm::vec4 ds(0);
	glm::vec4 dt(0);
	glm::vec4 dT(-3 * (1 - t) * (1 - t), 3 * (1 - t) * (1 - t) - 6 * t * (1 - t) , 6 * t * (1 - t) - 3 * t * t, 3 * t * t);
	glm::vec4 dS(-3 * (1 - s) * (1 - s), 3 * (1 - s) * (1 - s) - 6 * s * (1 - s) , 6 * s * (1 - s) - 3 * s * s, 3 * s * s);
	glm::vec4 S(s * s * s, s * s, s, 1);
	glm::vec4 T(t * t * t, t * t, t, 1);
	glm::vec4 SM = S * M;
	glm::vec4 TM = T * M;
	for (int i = 0; i < 4; i++)
	{
		p0_idx = point_idxs[i][0];
		p1_idx = point_idxs[i][1];
		p2_idx = point_idxs[i][2];
		p3_idx = point_idxs[i][3];

		Pi = glm::transpose(glm::mat4(controlPoints[p0_idx], controlPoints[p1_idx], controlPoints[p2_idx],
			controlPoints[p3_idx]));
		dt += dT[i] * SM * Pi;
		p0_idx = point_idxs[0][i];
		p1_idx = point_idxs[1][i];
		p2_idx = point_idxs[2][i];
		p3_idx = point_idxs[3][i];

		Pi = glm::transpose(glm::mat4(controlPoints[p0_idx], controlPoints[p1_idx], controlPoints[p2_idx],
			controlPoints[p3_idx]));
		ds += dS[i] * TM * Pi;
	}
	
	// for (int j = 0; j < 4; j++)
	// {
	// 	p0_idx = point_idxs[0][j];
	// 	p1_idx = point_idxs[1][j];
	// 	p2_idx = point_idxs[2][j];
	// 	p3_idx = point_idxs[3][j];
	//
	// 	Pi = glm::transpose(glm::mat4(controlPoints[p0_idx], controlPoints[p1_idx], controlPoints[p2_idx],
	// 		controlPoints[p3_idx]));
	// 	ds += dS[j] * TM * Pi;
	// }
	
	glm::vec3 normal = glm::cross(glm::vec3(ds), glm::vec3(dt));
	if(glm::length(normal) > 0.00000001)
		normal = glm::normalize(normal);
	
	return -normal;
	
}

Bezier2D::~Bezier2D()
{
}

void Bezier2D::CalcControlPoints(const Bezier1D* c)
{
	int seg_num = c->GetSegmentsNum();
	glm::vec4 p0;
	glm::vec4 p3;
	glm::mat4 segment;
	float alpha = 360.f / circularSubdivision;
	glm::mat4 rot = glm::rotate(glm::mat4(1), -alpha, glm::vec3(1, 0, 0));
	for (int num_rot = 0; num_rot < circularSubdivision + 1; num_rot++)
	{
		for (int i = 0; i < seg_num * 3 + 1; i++)
		{
			//Create the z oriented segment
			p0 = c->GetControlPoint(i);
			if (glm::abs(p0.y) < 0.0000001) //if the point is on the x axis
			{
				segment = glm::mat4(p0, p0, p0, p0);
			}
			else
			{
				segment = getCircleArcSegment(p0, alpha);
			}
			for (int t = 0; t < num_rot; t++)
			{
				segment[0] = rot * segment[0];
				segment[1] = rot * segment[1];
				segment[2] = rot * segment[2];
				segment[3] = rot * segment[3];
			}
			//Push the z oriented points into the control points vector
			for (int j = 0; j < 4; j++)
			{
				controlPoints.push_back(segment[j]);
			}
		}
	}
}

glm::mat4 Bezier2D::getCircleArcSegment(glm::vec4 p0, float alpha)
{
	float r = p0.y;
	float x = p0.x;
	glm::vec2 z0(1, 0);

	float c = 4.f * (1.f - glm::cos(0.5 * glm::radians(alpha))) / (3.f * glm::sin(0.5 * glm::radians(alpha)));
	
	glm::vec2 z1(1, c);

	glm::vec2 z3(glm::cos(glm::radians(alpha)), glm::sin(glm::radians(alpha)));
	glm::vec2 z2 = z3 + glm::vec2(c * glm::sin(glm::radians(alpha)), -c * glm::cos(glm::radians(alpha)));

	p0 = glm::vec4(x, r * z0, 0);
	glm::vec4 p1(x, r * z1, 0);
	glm::vec4 p2(x, r * z2, 0);
	glm::vec4 p3(x, r * z3, 0);
	
	glm::mat4 segment = glm::mat4(p0, p1, p2, p3);

	return segment;
}

glm::vec4 Bezier2D::CalcPoint(float s, float t, glm::mat4 point_idxs)
{
	int p0_idx;
	int p1_idx;
	int p2_idx;
	int p3_idx;

	glm::mat4 Pi;
	glm::vec4 b(0);
	glm::vec4 T((1 - t) * (1 - t) * (1 - t), 3 * t * (1 - t) * (1 - t), 3 * t * t * (1 - t), t * t * t);
	glm::vec4 S(s * s * s, s * s, s, 1);
	glm::vec4 SM = S * M;
	for (int i = 0; i < 4; i++)
	{
		p0_idx = point_idxs[i][0];
		p1_idx = point_idxs[i][1];
		p2_idx = point_idxs[i][2];
		p3_idx = point_idxs[i][3];

		Pi = glm::transpose(glm::mat4(controlPoints[p0_idx], controlPoints[p1_idx], controlPoints[p2_idx],
		                              controlPoints[p3_idx]));
		
		b += T[i] * SM * Pi;
		// if (controlPoints[p1_idx].y < 0 && i == 0)
		// 	printVec(b);
	}

	return b;
}
