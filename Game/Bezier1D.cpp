#include "Bezier1D.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}


Bezier1D::Bezier1D(int segNum, int res, int mode, int viewport): Shape(CreateHalfCircle(segNum, res), false, mode,
                                                                       viewport)
{
	resT = res;
	M = glm::mat4(glm::vec4(-1, 3, -3, 1),
	              glm::vec4(3, -6, 3, 0),
	              glm::vec4(-3, 3, 0, 0),
	              glm::vec4(1, 0, 0, 0));
	segments = std::vector<glm::mat4>();
	CreateInitSegments(segNum);
	IndexedModel model = GetLine();
	mesh = new MeshConstructor(model, false);
}

void Bezier1D::MoveControlPoint(int segment, int indx, float dx, float dy, bool preserveC1)
{
	glm::vec4 p = GetControlPoint(segment, indx);
	if (preserveC1 && indx % 3 != 0)
	{
		glm::vec4 p0;
		if (indx == 1)
		{
			p0 = GetControlPoint(segment, indx - 1);
		}
		else if (indx == 2)
		{
			p0 = GetControlPoint(segment, indx + 1);
		}
		glm::vec4 v = glm::normalize(p - p0);
		glm::vec4 delta(dx, dy, 0, 0);
		glm::vec4 proj = glm::dot(delta, v) * v;
		dx = proj.x;
		dy = proj.y;
		if (glm::length(p + glm::vec4(dx, dy, 0, 0) - p0) < 0.0000001)
		{
			dx = 0;
			dy = 0;
		}
	}
	p.x += dx;
	p.y += dy;
	SetControlPoint(segment, indx, p);
}

void Bezier1D::SetControlPoint(int segment, int indx, glm::vec4 newPoint)
{
	glm::mat4 seg = glm::transpose(segments[segment]);
	seg[indx] = newPoint;
	segments[segment] = glm::transpose(seg);
}

void Bezier1D::RotateControlPoint(int segment, int indx, float angle, glm::vec4 v, bool preserveC1)
{
	glm::vec4 p = GetControlPoint(segment, indx);
	glm::vec4 pv = p - v;
	p = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1)) * pv + v;
	SetControlPoint(segment, indx, p);
	if (preserveC1 && !(segment == 0 && indx == 1) && !(segment == segments.size() - 1 && indx == 2))
	{
		glm::vec4 p_adj;
		if (indx == 1)
		{
			segment--;
			indx++;
		}
		else if (indx == 2)
		{
			segment++;
			indx--;
		}
		p_adj = GetControlPoint(segment, indx);
		glm::vec4 p_adj_v = p_adj - v;
		p_adj = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1)) * p_adj_v + v;
		SetControlPoint(segment, indx, p_adj);
	}
}

IndexedModel Bezier1D::GetLine()
{
	IndexedModel model;
	glm::vec3 color(0, 0, 1);

	float t;
	glm::vec4 bt;
	std::vector<LineVertex> vertices;
	std::vector<int> indices;
	int index = 0;
	glm::vec4 first_p = glm::transpose(segments[0])[0];
	vertices.emplace_back(glm::vec3(first_p.x, first_p.y, first_p.z), color);
	indices.push_back(index++);
	for (auto i = 0; i < segments.size(); i++)
	{
		for (auto j = 1; j < resT - 1; j++)
		{
			t = float(j) / float(resT);
			bt = glm::vec4(t * t * t, t * t, t, 1) * M * segments[i];
			vertices.emplace_back(glm::vec3(bt.x, bt.y, 0), color);

			indices.push_back(index++);
		}
		glm::vec4 first_p = glm::transpose(segments[i])[3];
		vertices.emplace_back(glm::vec3(first_p.x, first_p.y, first_p.z), color);
		indices.push_back(index++);
	}

	for (auto i = 0; i < vertices.size(); i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.indices.push_back(indices[i]);
	}

	return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
	return glm::transpose(segments[segment])[indx];
}

glm::vec4 Bezier1D::GetControlPoint(int pointIndx) const
{
	int segment = pointIndx / 3;
	int indx = pointIndx % 3;
	if (segment == segments.size())
	if (segment == segments.size())
	{
		segment = segment - 1;
		indx = 3;
	}
	return GetControlPoint(segment, indx);
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, int t)
{
	return glm::vec4(1);
}

glm::vec3 Bezier1D::GetVelosity(int segment, int t)
{
	return glm::vec3(1);
}

void Bezier1D::SplitSegment(int segment, int t)
{
	glm::mat4 points = glm::transpose(segments[segment]);
	glm::vec4 p0 = points[0];
	glm::vec4 p1 = points[1];
	glm::vec4 p2 = points[2];
	glm::vec4 p3 = points[3];

	glm::vec4 r3 = p3;
	glm::vec4 r2 = .5f * (p2 + p3);
	glm::vec4 r1 = .5f * (r2 + .5f * (p1 + p2));
	glm::vec4 l0 = p0;
	glm::vec4 l1 = .5f * (p0 + p1);
	glm::vec4 l2 = .5f * (l1 + .5f * (p1 + p2));
	glm::vec4 l3 = .5f * (l2 + r1);
	glm::vec4 r0 = l3;

	glm::mat4 left_seg(l0, l1, l2, l3);
	glm::mat4 right_seg(r0, r1, r2, r3);
	segments[segment] = glm::transpose(right_seg);
	auto it = segments.begin();
	for (int i = 0; i < segment; i++)
		++it;
	segments.insert(it, glm::transpose(left_seg));

	IndexedModel model = GetLine();
	mesh = new MeshConstructor(model, false);
}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
}

void Bezier1D::CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1, int type, bool updateLine)
{
	int segment = pointIndx / 3;
	int indx = pointIndx % 3;
	glm::vec4 p0;
	switch (type)
	{
	case translate:
		if (segment < segments.size())
			MoveControlPoint(segment, indx, dx, dy, preserveC1);
		if (indx == 0 && segment > 0)
			MoveControlPoint(segment - 1, 3, dx, dy, preserveC1);
		break;
	case rotate:
		if (pointIndx % 3 == 1)
			p0 = GetControlPoint(pointIndx - 1);
		else if (pointIndx % 3 == 2)
			p0 = GetControlPoint(pointIndx + 1);
		RotateControlPoint(segment, indx, -dx * 100.f, p0, preserveC1);
		break;
	default:
		break;
	}

	if (updateLine)
	{
		IndexedModel model = GetLine();
		mesh->ChangeLine(model);
	}
}

void Bezier1D::SegmentUpdate(int segment, float dx, float dy, bool preserveC1)
{
	for (int i = 0; i < 4; i++)
	{
		CurveUpdate(segment * 3 + i, dx, dy, false, translate, false);
	}
	if (preserveC1)
	{
		if(segment > 0)
			CurveUpdate(segment * 3 - 1, dx, dy, false, translate, false);
		if(segment < segments.size() - 1)
			CurveUpdate((segment + 1) * 3 + 1, dx, dy, false, translate, false);
	}

	IndexedModel model = GetLine();
	mesh->ChangeLine(model);
}

bool Bezier1D::InConvexHull(int segment, float x, float y)
{
	glm::mat4 points = glm::transpose(segments[segment]);

	glm::vec2 p0(points[0]);
	glm::vec2 p1(points[1]);
	glm::vec2 p2(points[2]);
	glm::vec2 p3(points[3]);
	glm::vec2 p(x, y);

	float m = (p0.y - p3.y) / (p0.x - p3.x);
	float b = p0.y - m * p0.x;
	float eps = .000000001f;
	if (glm::abs(m * p2.x + b - p2.y) < eps && glm::abs(m * p1.x + b - p1.y) < eps)
	{
		float delta = 0.12;
		glm::vec2 p_ul(p0.x, p0.y + delta);
		glm::vec2 p_dl(p0.x, p0.y - delta);
		glm::vec2 p_ur(p3.x, p3.y + delta);
		glm::vec2 p_dr(p3.x, p3.y - delta);
		return PointInTriangle(p, p_ul, p_dl, p_dr) || PointInTriangle(p, p_ul, p_ur, p_dr);
	}

	return PointInTriangle(p, p0, p1, p3) || PointInTriangle(p, p0, p2, p3);
}

float Bezier1D::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Bezier1D::PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
	float d1 = sign(pt, v1, v2);
	float d2 = sign(pt, v2, v3);
	float d3 = sign(pt, v3, v1);

	bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

void Bezier1D::Reinitialize(int seg_num)
{
	segments.clear();
	CreateInitSegments(seg_num);
	IndexedModel model = GetLine();
	mesh = new MeshConstructor(model, false);
}

Bezier1D::~Bezier1D()
{
}

IndexedModel Bezier1D::CreateHalfCircle(const int seg_num, const int res)
{
	IndexedModel model;

	const auto vertex_num = seg_num * res + 1;
	float x(0.0), y(0.0);
	std::vector<LineVertex> vertices;
	std::vector<int> indices;
	for (auto i = 0; i <= vertex_num; i++)
	{
		x = 2.0f * float(i) / float(vertex_num) - 1.0f;
		y = glm::sqrt(1.0f - x * x);
		vertices.emplace_back(glm::vec3(x / 2.0f, y / 2.0f, 0), glm::vec3(1, 0, 0));
		indices.push_back(i);
	}

	for (auto i = 0; i <= vertex_num; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.indices.push_back(indices[i]);
	}
	return model;
}

void Bezier1D::CreateInitSegments(int seg_num)
{
	const auto vertex_num = seg_num * resT + 1;
	float x0;
	float y0;
	float x3;
	float y3;
	float mx = seg_num == 2 ? 0 : 0.5f;
	int ratio = seg_num - 2;
	float delta = 1.f / ratio;
	for (auto i = 0; i < seg_num; i++)
	{
		glm::mat4 segment;
		if (i == 0 || i == seg_num - 1)
		{
			if (i == 0)
			{
				x0 = seg_num == 2 ? -0.5f : -1.f;
				y0 = 0;
				x3 = seg_num == 2 ? 0.f : -.5f;
				y3 = .5f;
				segment = getHalfCircleSegment(x0, y0, x3, y3, -mx);
			}
			else if (i == seg_num - 1)
			{
				x0 = seg_num == 2 ? 0 : .5f;
				y0 = .5f;
				x3 = seg_num == 2 ? 0.5 : 1.f;
				y3 = 0;
				segment = getHalfCircleSegment(x0, y0, x3, y3, mx);
			}
		}
		else
		{
			glm::vec4 p0(-.5f + delta * (i - 1), .5f, 0, 0);
			glm::vec4 p1(-.5f + delta * (i - 1) + delta / 4, .5f, 0, 0);
			glm::vec4 p2(-.5f + delta * (i - 1) + 3 * delta / 4 , .5f, 0, 0);
			glm::vec4 p3(-.5f + delta * (i - 1) + 4 * delta / 4, .5f, 0, 0);
			segment = glm::transpose(glm::mat4(p0, p1, p2, p3));
		}
		segments.push_back(segment);
	}
}

glm::mat4 Bezier1D::getHalfCircleSegment(float x0, float y0, float x3, float y3, float mx)
{
	glm::vec4 p0(x0, y0, 0, 0);
	glm::vec4 p3(x3, y3, 0, 0);

	const auto ax = x0 - mx;
	const auto ay = y0;
	const auto bx = x3 - mx;
	const auto by = y3;
	const auto r = 0.5;
	const auto d = glm::sqrt((ax + bx) * (ax + bx) + (ay + by) * (ay + by));

	const auto k = (ax + bx) * (r / d - .5f) * (8.f / 3.f) / (ay - by);

	auto x1 = x0 + k * y0;
	auto y1 = y0 - k * (x0 - mx);

	auto x2 = x3 - k * y3;
	auto y2 = y3 + k * (x3 - mx);


	glm::vec4 p1(x1, y1, 0, 0);
	glm::vec4 p2(x2, y2, 0, 0);

	auto segment = glm::transpose(glm::mat4(p0, p1, p2, p3));

	return segment;
}
