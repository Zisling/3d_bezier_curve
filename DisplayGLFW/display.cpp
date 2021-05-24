#define GLEW_STATIC
#include "GL/glew.h"
#include <iostream>
#include "display.h"

Display::Display(int width, int height, const std::string& title)
{
	/* Initialize the library */
    if (!glfwInit())
        error =  -1;

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if(!m_window)
	{
		glfwTerminate();
		error = -1;
	}
	glfwMakeContextCurrent(m_window);

	GLenum res = glewInit();
    if(res != GLEW_OK)
   {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }
	isFullScreen = false;


}

void Display::AddKeyCallBack(void(*keyCallback)(GLFWwindow *,int,int,int,int))
{
	glfwSetKeyCallback(m_window,(void(*)(GLFWwindow *,int,int,int,int))keyCallback);//{

}

void Display::AddMouseCallBacks(void (* mousebuttonfun)(GLFWwindow*,int,int,int),void (* scrollfun)(GLFWwindow*,double,double),void (* cursorposfun)(GLFWwindow*,double,double))
{
	glfwSetMouseButtonCallback(m_window,mousebuttonfun);
	glfwSetScrollCallback(m_window, scrollfun);
	glfwSetCursorPosCallback(m_window , cursorposfun);
}

void Display::AddResizeCallBack(void (*windowsizefun)(GLFWwindow*,int,int))
{
	glfwSetWindowSizeCallback(m_window,windowsizefun);
}

//void Display::SetScene(void *userPointer)
//{
//	
//	glfwSetWindowUserPointer(m_window,userPointer);
//}

void Display::SetRenderer(void* userPointer)
{

	glfwSetWindowUserPointer(m_window, userPointer);
}
void* Display::GetScene()
{
	return glfwGetWindowUserPointer(m_window);
}

bool Display::CloseWindow()
{
	return glfwWindowShouldClose(m_window) !=0;
}

Display::~Display()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Display::IsFullscreen( void )
{
	if((glfwGetWindowMonitor( m_window ) == nullptr) != isFullScreen)
	{
		isFullScreen = (glfwGetWindowMonitor( m_window ) == nullptr);
		return true;
	}
	else 
		return false;
} 

void Display::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Display::PollEvents()
{
	glfwPollEvents();
}
