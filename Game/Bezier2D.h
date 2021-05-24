#pragma once


#include "Bezier1D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Mesh.h"

class Bezier2D : public Shape
{
	int circularSubdivision; //usualy 4 how many subdivision in circular direction
	std::vector<glm::vec4> controlPoints;
	int resT;
	int resS;
	int subNum;
	glm::mat4 M = glm::mat4(glm::vec4(-1, 3, -3, 1),
	                        glm::vec4(3, -6, 3, 0),
	                        glm::vec4(-3, 3, 0, 0),
	                        glm::vec4(1, 0, 0, 0));

	void AddToModel(IndexedModel& model, float s, float t, const std::vector<glm::vec4> subSurf, int subIndx);
	// add a new Nurb to the model
	glm::vec4 CalcNurbs(float s, float t, const std::vector<glm::vec4> subSurf);
	glm::vec3 CalcNormal(float s, float t, const std::vector<glm::vec4> subSurf);
	void CalcControlPoints(const Bezier1D* c); // calculates control points cubic Bezier manifold.

	glm::vec4 CalcPoint(float s, float t, glm::mat4 point_idxs);
	glm::mat4 getCircleArcSegment(glm::vec4 , float alpha);
public:
	Bezier2D();
	Bezier2D(const Bezier1D* c, int circular_subdivision, int res_s, int res_t, int mode, int viewport);
	IndexedModel GetSurface(); //generates a model for MeshConstructor Constructor
	Vertex* GetPointOnSurface(int segmentS, int segmentT, int s, int t);
	//returns a point on the surface in the requested segment for value of t and s
	glm::vec3 GetNormal(float s, float t, glm::mat4 );
	//returns a normal of a point on the surface in the requested segment for value of t and s

	std::vector<glm::vec4> GetControlPoints() const
	{
		return controlPoints;
	}

	//void MoveControlPoint(int segmentS, int segmentT, int indx, bool preserveC1);
	~Bezier2D();
};
