#pragma once

#include "Mesh.h"
#include "MeshConstructor.h"
#include "shape.h"


enum axis { xAxis, yAxis, zAxis };
	enum transformations { xTranslate, yTranslate, zTranslate, xRotate, yRotate, zRotate, xScale, yScale, zScale, xCameraTranslate, yCameraTranslate, zCameraTranslate };
	enum modes { POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUADS };
	enum shapes { Axis, Plane, Cube, Octahedron, Tethrahedron, LineCopy, MeshCopy };
	enum buffers { COLOR, DEPTH, STENCIL, BACK, FRONT, NONE };
enum moveType { translate, rotate };

class Bezier1D : public Shape
{
	std::vector<glm::mat4> segments;
	int resT;
	glm::mat4 M;
	void MoveControlPoint(int segment, int indx, float dx, float dy, bool preserveC1);
	//change the position of one control point. when preserveC1 is true it may affect other  control points
	void SetControlPoint(int segment, int indx, glm::vec4 newPoint);
	void RotateControlPoint(int segment, int indx, float angle, glm::vec4 v, bool preserveC1);
public:
	Bezier1D(int segNum, int res, int mode, int viewport = 0);

	IndexedModel GetLine(); //generates a model for MeshConstructor Constructor
	glm::vec4 GetControlPoint(int segment, int indx) const;
	//returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3

	glm::vec4 GetControlPoint(int pointIndx) const;

	glm::vec4 GetPointOnCurve(int segment, int t); //returns point on curve in the requested segment for the value of t
	glm::vec3 GetVelosity(int segment, int t);
	//returns the derivative of the curve in the requested segment for the value of t
	void SplitSegment(int segment, int t); // split a segment into two parts
	void AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3); // add a segment at the end of the curve
	void CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1, int type, bool updateLine);
	//change the line in by using ChangeLine function of MeshConstructor and MoveControlPoint
	void SegmentUpdate(int segment, float dx, float dy, bool preserveC1);
	bool InConvexHull(int segment, float x, float y);
	inline int GetSegmentsNum() const { return segments.size(); }

	void Reinitialize(int seg_num);

	static float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	static bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
	~Bezier1D(void);
protected:

	static IndexedModel CreateHalfCircle(const int seg_num, const int res);
	void CreateInitSegments(int seg_num);
	static glm::mat4 getHalfCircleSegment(float x0, float y0, float x3, float y3, float mx);
};
