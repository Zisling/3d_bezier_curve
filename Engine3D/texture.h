#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

class Texture
{
public:
	Texture(const std::string& fileName,const int dim);
	Texture(int width, int height,unsigned char *data);
	Texture(int width, int height);
	void Bind(int slot);
	static void load_cubemap(std::vector<std::string> faces);

	inline int GetSlot(){return m_texture;}
	 ~Texture();
protected:
private:
	//Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
	unsigned int m_texture;
	int texDimention;

	const std::vector<std::string> m_faces =
	{
		"../res/textures/right.jpg",
		"../res/textures/left.jpg",
		"../res/textures/top.jpg",
		"../res/textures/bottom.jpg",
		"../res/textures/front.jpg",
		"../res/textures/back.jpg"
	};
};

#endif
