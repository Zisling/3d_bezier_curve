#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include "GLFW/glfw3.h"
#include <string>

class Display
{
	bool isFullScreen;
public:
	Display(int width, int height, const std::string& title);

	void SwapBuffers();
	void PollEvents();
	
//	void SetScene(void *userPointer);
	void SetRenderer(void* userPointer);

	void* GetScene();
	void AddKeyCallBack(void(*func)(GLFWwindow *,int,int,int,int));
	void AddMouseCallBacks(void (* mousebuttonfun)(GLFWwindow*,int,int,int),void(* scrollfun)(GLFWwindow*,double,double),void (* cursorposfun)(GLFWwindow*,double,double));
	void AddResizeCallBack(void (*windowsizefun)(GLFWwindow*,int,int));
	
	bool IsFullscreen(  );

	bool CloseWindow();
	 ~Display();
//protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	GLFWwindow* m_window;
	int error;
};

#endif
