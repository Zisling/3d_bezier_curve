#include "MovableGLM.h"
#include <stdio.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

MovableGLM::MovableGLM()
{
	ZeroTrans();
}

glm::mat4 MovableGLM::MakeTrans(const glm::mat4& prevTransformations) const
{
	return prevTransformations * MakeTrans();
}

glm::mat4 MovableGLM::MakeTrans() const
{
	return rot * trans * scl;
}

void MovableGLM::MyTranslate(const glm::vec3 delta, int mode)
{
	if (mode == 0)
		trans = glm::translate(trans, delta);
	else if (mode == 1)
		trans = glm::translate(glm::mat4(1), delta);
	else if (mode == 2)
		trans = glm::translate(glm::mat4(1), glm::vec3(glm::inverse(rot) * glm::vec4(delta, 0))) * trans;
}

void MovableGLM::MyRotate(float angle, const glm::vec3& vec, int mode)
{
	if (mode == 0)
		rot = glm::rotate(glm::mat4(1), angle, vec) * rot;
	else if (mode == 1)
		rot = glm::rotate(rot, angle, glm::vec3(glm::transpose(rot) * glm::vec4(vec, 0)));
	else if (mode == 2)
	{
		glm::mat4 vtrans = glm::translate(glm::mat4(1), vec);
		rot = vtrans * glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1)) * glm::inverse(vtrans) * rot;
	}
	else if (mode == 3)
	{
		rot = rot * trans * glm::rotate(glm::mat4(1), angle, glm::vec3(glm::inverse(rot) * glm::vec4(vec, 0))) *
			glm::inverse(trans);
	}
	else if (mode == 4)
	{
		rot = rot * trans * glm::rotate(glm::mat4(1), angle, vec) * glm::inverse(trans);
	}
}

void MovableGLM::MyScale(const glm::vec3 scale)
{
	scl = glm::scale(scl, scale);
}

void MovableGLM::ZeroTrans()
{
	trans = glm::mat4(1);
	rot = glm::mat4(1);
	scl = glm::mat4(1);
}
