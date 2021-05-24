Basic 3D graphics engine - based on the Opengl series of TheCherno 

Initialization:
0. Make sure you download GLFW version suitable to your compiler. 
1. inputManager.h:
   1. Init function which define keyboard, mouse and resize functions (see inputManager attached)
   2. Callback functions implementation (keyboard, mouse and resize) 
      inputManager may be change when display library changes (now we use GLFW)
2. main.cpp
   You can use the main file supported in Game project. 
   In main.cpp you should do the following:
   1. Define the width and height of the screen and camera properties (openning angle,near and far)
   2. Define Display object that will create the screen
   3. Call inputManager.h Init function 
   4. Define Scene pointer and call init your function.
   5. Call Display.SetScene to attache Scene pointer to Display's window. 
   6. Main loop: must contain Scene->Draw, Display.SwapBuffers and Display.PollEvents
	
3. A class inherits from scene (.h and .cpp files)
   1. You may implement the virtual function (see list bellow)
   2. Add init function to initiate your scene.

Virtual function description 
1. Update - function that define which uniform values will upload to the shader.
	    must be implemented on the inherited class. 
	    Use SetUniform... functions to define a new uniform. 
2. Draw  -  function that calculates uniform values for every shape and call update.
	    This function will submit every shape to the rendering pipeline by calling shape->Draw.  
3. WhenRotate - should call every time a shape rotates. 
4. WhenTranslate - should call every time a shape change position.
5. Motion - calls every frame (may use for animation)

Add attribute, type vec2 or vec3
1. In shader.h add attribute name in enum Attributes
2. In shader.cpp bind your new attribute in the constructor by "glBindAttribLocation"
3. In MeshConstructor.h add VEC2_ATTRIB_NUM or VEC3_ATTRIB_NUM 1
4. In Mesh.h in GetData function add attribute in the appropriate location (indieces must remain the last one)
5. In Mesh.h add Vertex or LineVertex the new attribute 
6. If you want to add attribute to the simple objects you can do so in Mesh.cpp


