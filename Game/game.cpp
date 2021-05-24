#include "game.h"
#include <iostream>
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/_vectorize.hpp>



#include "Bezier1D.h"
#include "Bezier1D.h"
#include "Bezier1D.h"
#include "Bezier1D.h"
#include "stb_image.h"
#include "GL/glew.h"

static void printVec(const glm::vec4 vec)
{
	std::cout << " vector:" << std::endl;
	for (int j = 0; j < 4; j++)
		std::cout << vec[j] << " ";
	std::cout << std::endl;
}

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

static bool inRectangle(glm::vec2 p, float x, float y, float width, float height)
{
	return p.x > x && p.x < x + width && p.y > y && p.y < y + height;
}

Game::Game() : Scene(), m_num_segments(3),
               m_bezier1d(nullptr),
               m_preserveC1(false)
{
}

void Game::Init()
{
	unsigned int texIDs[3] = {0, 1, 0};
	unsigned int slots[3] = {0, 1, 0};

	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader2");
	AddShader("../res/shaders/basicShader");
	AddShader("../res/shaders/skyboxShader");
	AddShader("../res/shaders/lightingShader");
	AddShader("../res/shaders/stencilShader");

	AddTexture("../res/textures/box0.bmp", 2);
	AddTexture("cubemap", 6);

	AddMaterial(texIDs, slots, 2);
	AddMaterial(texIDs + 1, slots + 1, 2);

	AddShape(Plane, -1, TRIANGLES);
	m_scissor_plane_idx = shapes.size() - 1;
	pickedShape = m_scissor_plane_idx;
	ShapeTransformation(zTranslate, -0.11f);
	SetShapeShader(m_scissor_plane_idx, 2);
	AddShapeViewport(m_scissor_plane_idx, 2);
	RemoveShapeViewport(m_scissor_plane_idx, 0);

	//add skybox to the first screen
	AddShape(Cube, -1, TRIANGLES);
	m_skybox_idx = shapes.size() - 1;
	SetShapeShader(m_skybox_idx, 3);
	//enlarge the skybox
	pickedShape = m_skybox_idx;
	ShapeTransformation(scale, 10);

	//add x,y axis to the second screen
	AddShape(Axis, -1, LINES);
	m_axis_idx = shapes.size() - 1;
	AddShapeViewport(m_axis_idx, 1);
	RemoveShapeViewport(m_axis_idx, 0);
	//enlarge the axis
	pickedShape = m_axis_idx;
	ShapeTransformation(scale, 100);

	//add the bezier curve to the second screen
	m_bezier1d = new Bezier1D(m_num_segments, 50, LINE_STRIP, 2);
	AddShape(m_bezier1d, -1);
	m_bezier_idx = shapes.size() - 1;
	//add octahedrons that correspond with the control points of the bezier curve
	AddShapesForInitialBezier();
	pickedShape = -1;
}

void Game::AddShapesForInitialBezier()
{
	for (int i = 0; i < m_num_segments; i++)
	{
		for (int j = i == 0 ? 0 : 1; j < 4; j++)
		{
			glm::vec4 p = m_bezier1d->GetControlPoint(i, j);
			AddShape(Octahedron, -1, TRIANGLES);
			pickedShape = shapes.size() - 1;
			AddShapeViewport(pickedShape, 1);
			RemoveShapeViewport(pickedShape, 0);
			ShapeTransformation(xTranslate, p.x);
			ShapeTransformation(yTranslate, p.y);
			ShapeTransformation(scale, .03f);
			m_shape_idx_to_control[pickedShape] = i * 3 + j;
			m_control_to_shape_idx.push_back(pickedShape);
		}
	}
	pickedShape = -1;
}

void Game::Update(const glm::mat4& MVP, const glm::mat4& Model, const int shaderIndx, const glm::vec4 camPos)
{
	Shader* s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	if (shapes[pickedShape]->GetMaterial() >= 0 && !materials.empty())
		BindMaterial(s, shapes[pickedShape]->GetMaterial());
	//textures[0]->Bind(0);
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Model);

	if (shaderIndx == 0 && pickedShape != m_skybox_idx)
		s->SetUniform4f("lightColor", float(r) / 255.f, 0, 0, 1);
	else if (shaderIndx == 0 && pickedShape == m_skybox_idx)
		s->SetUniform4f("lightColor", 0, 0, 0, 1);
	s->SetUniform4f("camPos", camPos.x, camPos.y, camPos.z, camPos.w);
	s->SetUniform1i("sampler1", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(0));
	if (shaderIndx == 3)
		s->SetUniform1i("skybox", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	s->Unbind();
}

void Game::WhenRotate()
{
	const auto element = m_shape_idx_to_control.find(pickedShape);
	if (element != m_shape_idx_to_control.end())
	{
		const int pointIndx = element->second;
		RotateControlPoint(pointIndx, pointIndx % 3 == 0);
	}
	else if (pickedShape != m_skybox_idx && pickedShape != m_scissor_plane_idx && pickedShape != m_axis_idx)
	{
		shapes[pickedShape]->MyRotate(m_dy * 100, glm::vec3(1, 0, 0), 3);
		shapes[pickedShape]->MyRotate(-m_dx * 50, glm::vec3(0, 1, 0), 3);
	}
}

void Game::RotateControlPoint(int pointIndx, bool onLine)
{
	if (!onLine)
	{
		glm::vec4 p = m_bezier1d->GetControlPoint(pointIndx);
		glm::vec4 p0;
		if (pointIndx % 3 == 1)
			p0 = m_bezier1d->GetControlPoint(pointIndx - 1);
		else if (pointIndx % 3 == 2)
			p0 = m_bezier1d->GetControlPoint(pointIndx + 1);

		m_bezier1d->CurveUpdate(pointIndx, m_dx, 0, m_preserveC1, rotate, true);
		p = m_bezier1d->GetControlPoint(pointIndx) - p;
		shapes[pickedShape]->MyTranslate(glm::vec3(p), 0);

		if (m_preserveC1 && pointIndx != 1 && pointIndx != m_num_segments * 3)
		{
			if (pointIndx % 3 == 1)
			{
				pointIndx -= 2;
			}
			else if (pointIndx % 3 == 2)
			{
				pointIndx += 2;
			}
			pickedShape = m_control_to_shape_idx[pointIndx];
			glm::vec4 p_adj = m_bezier1d->GetControlPoint(pointIndx);
			shapes[pickedShape]->MyTranslate(glm::vec3(p_adj), 1);
		}
	}
	else if (pointIndx > 0 && pointIndx < m_num_segments * 3 && !m_preserveC1)
	{
		glm::vec4 p0 = m_bezier1d->GetControlPoint(pointIndx);
		glm::vec4 p2_prev = m_bezier1d->GetControlPoint(pointIndx - 1);
		glm::vec4 p1_next = m_bezier1d->GetControlPoint(pointIndx + 1);
		glm::vec4 line = glm::normalize(p0 - p2_prev);
		float t = glm::length(p0 - p1_next);
		glm::vec4 p1_next_new = p0 + t * line;
		glm::vec4 delta = p1_next_new - p1_next;
		m_bezier1d->CurveUpdate(pointIndx + 1, delta.x, delta.y, m_preserveC1, translate, true);
		pickedShape = m_control_to_shape_idx[pointIndx + 1];
		shapes[pickedShape]->MyTranslate(glm::vec3(delta), 0);
	}
}


void Game::WhenTranslate()
{
	const auto element = m_shape_idx_to_control.find(pickedShape);
	if (element != m_shape_idx_to_control.end())
	{
		const int pointIndx = element->second;
		int current_shape = pickedShape;
		bool moved = TranslatePoint(pointIndx, false, pointIndx % 3 == 0);
		if (pointIndx % 3 == 0 && moved)
		{
			bool toggle = m_preserveC1;
			if (toggle)
				TogglePreserve();
			if (pointIndx < m_num_segments * 3)
			{
				pickedShape = m_control_to_shape_idx[pointIndx + 1];
				TranslatePoint(pointIndx + 1, true, false);
			}
			if (pointIndx > 0)
			{
				pickedShape = m_control_to_shape_idx[pointIndx - 1];
				TranslatePoint(pointIndx - 1, true, false);
			}
			if (toggle)
				TogglePreserve();
			pickedShape = current_shape;
		}
	}
	else if (pickedShape != m_skybox_idx && pickedShape != m_axis_idx)
	{
		for (auto picked_index : pickedIndices)
		{
			shapes[picked_index]->MyTranslate(glm::vec3(m_dx, m_dy, 0), 2);
		}
	}
}

bool Game::TranslatePoint(int pointIndx, bool useDelta, bool onLine)
{
	glm::vec4 p = m_bezier1d->GetControlPoint(pointIndx);

	if (m_y > 0 || !onLine)
	{
		if (useDelta || m_preserveC1)
		{
			if (onLine && p.y + m_dy < 0)
				m_dy = -p.y;

			m_bezier1d->CurveUpdate(pointIndx, m_dx, m_dy, m_preserveC1, translate, true);
			p = m_bezier1d->GetControlPoint(pointIndx);
			shapes[pickedShape]->MyTranslate(glm::vec3(p), 1);
		}
		else
		{
			m_bezier1d->CurveUpdate(pointIndx, m_x - p.x, m_y - p.y, m_preserveC1, translate, true);
			shapes[pickedShape]->MyTranslate(glm::vec3(m_x, m_y, 0), 1);
		}
		return true;
	}
	return false;
}

void Game::MouseProccessing(int button, bool isCursPos)
{
	if (pickedShape != -1 && pickedShape != m_bezier_idx)
	{
		if (button == 1)
		{
			WhenTranslate();
		}
		else
		{
			WhenRotate();
		}
	}
	else
	{
		if (button == 1)
		{
			MoveSegmentCommand();
		}
		if (button == 0 && m_num_segments < 6 && !isCursPos)
		{
			SplitSegmentCommand();
		}
	}
}

void Game::MoveSegmentCommand()
{
	for (int i = 0; i < m_num_segments; i++)
	{
		if (m_bezier1d->InConvexHull(i, m_x, m_y))
		{
			glm::vec4 p0 = m_bezier1d->GetControlPoint(i, 0);
			glm::vec4 p3 = m_bezier1d->GetControlPoint(i, 3);

			if (p0.y + m_dy < 0)
				m_dy = -p0.y;
			if (p3.y + m_dy < 0)
				m_dy = -p3.y;
			m_bezier1d->SegmentUpdate(i, m_dx, m_dy, m_preserveC1);
			glm::vec4 p;
			for (int ii = 0; ii < 4; ii++)
			{
				p = m_bezier1d->GetControlPoint(i, ii);
				pickedShape = m_control_to_shape_idx[i * 3 + ii];
				shapes[pickedShape]->MyTranslate(glm::vec3(p), 1);
			}
			if (m_preserveC1)
			{
				if (i > 0)
				{
					p = m_bezier1d->GetControlPoint(i - 1, 2);
					pickedShape = m_control_to_shape_idx[i * 3 - 1];
					shapes[pickedShape]->MyTranslate(glm::vec3(p), 1);
				}

				if (i < m_num_segments - 1)
				{
					p = m_bezier1d->GetControlPoint(i + 1, 1);
					pickedShape = m_control_to_shape_idx[(i + 1) * 3 + 1];
					shapes[pickedShape]->MyTranslate(glm::vec3(p), 1);
				}
			}
			pickedShape = -1;
			break;
		}
	}
}

void Game::Motion()
{
	if (isActive)
	{
	}
}

unsigned int Game::TextureDesine(int width, int height)
{
	unsigned char* data = new unsigned char[width * height * 4];
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			data[(i * height + j) * 4] = (i + j) % 256;
			data[(i * height + j) * 4 + 1] = (i + j * 2) % 256;
			data[(i * height + j) * 4 + 2] = (i * 2 + j) % 256;
			data[(i * height + j) * 4 + 3] = (i * 3 + j) % 256;
		}
	}
	textures.push_back(new Texture(width, height));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//note GL_RED internal format, to save 
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] data;
	return (textures.size() - 1);
}

Game::~Game(void)
{
}

void Game::UpdatePosition(float xpos, float ypos)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float oldx = m_x;
	float oldy = m_y;
	m_x = m_ortho_project * (2 * xpos / float(viewport[2]) - 3);
	m_y = m_ortho_project * (2 * (1 - ypos / float(viewport[3])) - 1);

	m_xpos = xpos;
	m_dx = m_x - oldx;
	m_dy = m_y - oldy;
}

bool Game::InBezConvexHull()
{
	return m_bezier1d->InConvexHull(0, m_x, m_y);
}

void Game::ValidateShapeIdxToControlMap()
{
	int shape_indx;
	for (int j = 0; j < m_control_to_shape_idx.size(); j ++)
	{
		shape_indx = m_control_to_shape_idx[j];
		m_shape_idx_to_control[shape_indx] = j;
	}
}

void Game::AddNewControlShapes(int r_seg)
{
	int r0_point = r_seg * 3;
	auto it = m_control_to_shape_idx.begin();
	for (int j = 0; j <= r0_point; j++)
		++it;
	m_control_to_shape_idx.insert(it, 3, -1);

	int r_point;
	glm::vec4 r_p;
	int shp_indx = shapes.size();
	for (int j = 1; j < 4; j++)
	{
		glm::vec4 p = m_bezier1d->GetControlPoint(r_seg, j);
		AddShape(Octahedron, -1, TRIANGLES);
		pickedShape = shp_indx;
		AddShapeViewport(shp_indx, 1);
		RemoveShapeViewport(shp_indx, 0);
		ShapeTransformation(xTranslate, p.x);
		ShapeTransformation(yTranslate, p.y);
		ShapeTransformation(scale, .03f);
		m_control_to_shape_idx[r_seg * 3 + j] = shp_indx;
		shp_indx++;
	}
}

void Game::UpdateOldControlShapesToNewPositions(int l_seg)
{
	int l_point;
	glm::vec4 l_p;
	for (int j = 0; j < 4; j++)
	{
		l_point = l_seg * 3 + j;
		l_p = m_bezier1d->GetControlPoint(l_point);
		pickedShape = m_control_to_shape_idx[l_point];
		shapes[pickedShape]->MyTranslate(glm::vec3(l_p), 1);
	}
}

void Game::SplitSegmentCommand()
{
	for (int i = 0; i < m_num_segments; i++)
	{
		if (m_bezier1d->InConvexHull(i, m_x, m_y))
		{
			m_bezier1d->SplitSegment(i, -1);
			int l_seg = i;
			int r_seg = i + 1;

			UpdateOldControlShapesToNewPositions(l_seg);

			AddNewControlShapes(r_seg);

			ValidateShapeIdxToControlMap();

			m_num_segments++;
			pickedShape = -1;
			break;
		}
	}
}

void Game::TogglePreserve()
{
	m_preserveC1 = !m_preserveC1;
}

void Game::ScrollProcessing(float dscroll, bool curve_screen)
{
	if (curve_screen)
	{
		m_ortho_project = dscroll;
	}
	else
	{
		for (auto picked_index : pickedIndices)
			shapes[picked_index]->MyTranslate(glm::vec3(0, 0, -dscroll * 0.1), 2);
	}
}

void Game::ReinitializeBezier1D(int seg_num)
{
	m_num_segments = seg_num;
	m_bezier1d->Reinitialize(seg_num);

	for (int i : m_control_to_shape_idx)
	{
		RemoveShapeViewport(i, 1);
	}

	m_control_to_shape_idx.clear();
	m_shape_idx_to_control.clear();
	AddShapesForInitialBezier();
}

void Game::Add2DBezier()
{
	Bezier2D* bez2d = new Bezier2D(m_bezier1d, 4, 100, 50, QUADS, 1);
	AddShape(bez2d, -1);
	pickedShape = shapes.size() - 1;
	ShapeTransformation(zTranslate, -3);
	SetShapeShader(pickedShape, 4);
	m_bezier_2ds.push_back(bez2d);
	m_bezier_2d_idxs.push_back(pickedShape);
}


void Game::PickScissorArea(int x, int y, int width, int height, glm::mat4 camProj)
{
	glm::vec4 pos;
	x = width > 0 ? x : x + width;
	y = height > 0 ? y : y + height;
	width = abs(width);
	height = abs(height);
	float x_n = 2.f * float(x) / 800.f - 1.f;
	float y_n = 2.f * float(y) / 800.f - 1.f;
	float width_n = 2 * float(width) / 800.f;
	float height_n = 2 * float(height) / 800.f;

	for (auto shape_index : m_bezier_2d_idxs)
	{
		pos = camProj * shapes[shape_index]->MakeTrans() * glm::vec4(0, 0, 0, 1);
		pos /= pos.z;
		
		if (inRectangle(glm::vec2(pos), x_n, y_n, width_n, height_n))
			pickedIndices.insert(shape_index);
	}
}
