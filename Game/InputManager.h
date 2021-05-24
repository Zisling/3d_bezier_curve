#pragma once   //maybe should be static class

#include "display.h"
#include "renderer.h"
#include "game.h"


void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Game* scn = (Game*)rndr->GetScene();
		double x2, y2;
		glfwGetCursorPos(window, &x2, &y2);
		rndr->ClearScissorMode();
		if (rndr->Picking((int)x2, (int)y2))
		{
			rndr->UpdatePosition(x2, y2);
			scn->UpdatePosition(float(x2), float(y2));
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			if(scn->screenNum() == 0 && (scn->GetPickedShape() == -1 || scn->GetPickedShape() == scn->GetSkyboxIdx()))
				rndr->SetScissor(x2, y2);
			else
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Game* scn = (Game*)rndr->GetScene();
	if (scn->screenNum() == 1)
	{
		rndr->MoveCamera(1, Renderer::zScale, -yoffset);
		scn->ScrollProcessing(rndr->getCamera(1)->GetOrthoProject(), true);
	}
	else
	{
		scn->ScrollProcessing(yoffset, false);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Game* scn = (Game*)rndr->GetScene();

	rndr->UpdatePosition((float)xpos, (float)ypos);
	scn->UpdatePosition((float)xpos, (float)ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (scn->screenNum() == 0 && (scn->GetPickedShape() == -1 || scn->GetPickedShape() == scn->GetSkyboxIdx()))
			rndr->UpdateScissor(xpos, ypos);
		else
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT, true);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT, true);
	}
	else
	{
		if (rndr->ClearScissor()) {
			rndr->PickScissorArea();
		}
	}
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

	rndr->Resize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
	Game* scn = (Game*)rndr->GetScene();

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_SPACE:
			scn->Add2DBezier();
			break;
		case GLFW_KEY_C:
			scn->TogglePreserve();
			break;
		case GLFW_KEY_R:
			rndr->MoveCamera(0, scn->xTranslate, 0.4f);
			break;
		case GLFW_KEY_L:
			rndr->MoveCamera(0, scn->xTranslate, -0.4f);
			break;
		case GLFW_KEY_U:
			rndr->MoveCamera(0, scn->yTranslate, 0.4f);
			break;
		case GLFW_KEY_D:
			rndr->MoveCamera(0, scn->yTranslate, -0.4f);
			break;
		case GLFW_KEY_B:
			rndr->MoveCamera(0, scn->zTranslate, 0.4f);
			break;
		case GLFW_KEY_F:
			rndr->MoveCamera(0, scn->zTranslate, -0.4f);
			break;
		case GLFW_KEY_UP:
			rndr->getCamera(0)->MyRotate(10, glm::vec3(1, 0, 0), 4);
			break;
		case GLFW_KEY_DOWN:
			rndr->getCamera(0)->MyRotate(-10, glm::vec3(1, 0, 0), 4);
			break;
		case GLFW_KEY_LEFT:
			rndr->getCamera(0)->MyRotate(10, glm::vec3(0, 1, 0), 3);
			break;
		case GLFW_KEY_RIGHT:
			rndr->getCamera(0)->MyRotate(-10, glm::vec3(0, 1, 0), 3);
			break;
		case GLFW_KEY_2:
			scn->ReinitializeBezier1D(2);
			break;
		case GLFW_KEY_3:
			scn->ReinitializeBezier1D(3);
			break;
		case GLFW_KEY_4:
			scn->ReinitializeBezier1D(4);
			break;
		case GLFW_KEY_5:
			scn->ReinitializeBezier1D(5);
			break;
		case GLFW_KEY_6:
			scn->ReinitializeBezier1D(6);
			break;
		default:
			break;
		}
	}
}

void Init(Display& display)
{
	display.AddKeyCallBack(key_callback);
	display.AddMouseCallBacks(mouse_callback, scroll_callback, cursor_position_callback);
	display.AddResizeCallBack(window_size_callback);
}
