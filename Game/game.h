#pragma once


#include "Bezier1D.h"
#include "scene.h"
#include "Bezier2D.h"

class Game : public Scene
{
public:

	Game();
	void AddShapesForInitialBezier();
	void Init();
	void Update(const glm::mat4& MVP, const glm::mat4& Model, const int  shaderIndx, const glm::vec4 camPos);

	void RotateControlPoint(int pointIndx, bool onLine);
	void WhenRotate();
	void WhenTranslate();
	bool TranslatePoint(int pointIndx, bool useDelta, bool onLine);
	void Motion();

	unsigned int TextureDesine(int width, int height);
	~Game(void);
	void UpdatePosition(float xpos, float ypos);

	bool InBezConvexHull();
	void ValidateShapeIdxToControlMap();
	void AddNewControlShapes(int r_seg);
	void UpdateOldControlShapesToNewPositions(int l_seg);
	void SplitSegmentCommand();
	void MouseProccessing(int button, bool isCursPos);
	
	void MoveSegmentCommand();
	
	void TogglePreserve();
	void ScrollProcessing(float dscroll, bool curve_screen);

	int screenNum() const
	{
		if(m_xpos > 800)
		{
			return 1;
		}
		return 0;
	}

	void Add2DBezier();

	void ReinitializeBezier1D(int seg_num);

	int GetSkyboxIdx() const
	{
		return m_skybox_idx;
	}

	void PickScissorArea(int x, int y, int width, int height, glm::mat4);
private:
	std::vector<int> m_control_to_shape_idx{};
	std::unordered_map<int, int> m_shape_idx_to_control{};

	std::vector<Bezier2D *> m_bezier_2ds;
	std::vector<int> m_bezier_2d_idxs;
	
	float m_x{}, m_y{}, m_dx{}, m_dy{};
	float m_xpos;

	float m_ortho_project = 1.f;
	int m_bezier_idx{};
	int m_skybox_idx{};
	int m_axis_idx{};
	int m_num_segments;
	Bezier1D* m_bezier1d;
	bool m_preserveC1;
	int m_scissor_plane_idx{};
};