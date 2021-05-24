#pragma once
#include "glm/glm.hpp"

class MovableGLM 
{
public:
	MovableGLM();
	glm::mat4 MakeTrans(const glm::mat4 &prevTransformations) const;
	glm::mat4 MakeTrans() const;
	void MyTranslate(const glm::vec3 delta, int mode);
	void MyRotate(float angle, const glm::vec3 &vec, int mode);
	void MyScale(const glm::vec3 scale);
	void ZeroTrans();
private:
	glm::mat4 trans;
	glm::mat4 rot;
	glm::mat4 scl;
};
