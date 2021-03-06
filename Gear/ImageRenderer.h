#pragma once

#include "TextureAsset.h"
#include "ShaderProgram.h"
#include "Importer.h"
#include <vector>

struct sScreenImage
{
	sScreenImage()
	{
		glm::vec2 pos(0, 0);
		width = 0;
		height = 0;
	}
	sScreenImage(glm::vec2 pos, float width, float height)
	{
		this->pos = pos;
		this->width = width;
		this->height = height;
	}

	bool mousePick(float x,float y)
	{
		if (x > pos.x && x < pos.x + width)
			if (y > pos.y && y < pos.y + height)
				return true;
		return false;
	}

	glm::vec2 pos;
	float width;
	float height;
};

namespace Gear
{
	class ImageRenderer
	{
	public:
		GEAR_API ImageRenderer();
		GEAR_API ~ImageRenderer();

		GEAR_API void init(int screenWidth = 800, int screenHeight = 600);

		GEAR_API void showImage(const sScreenImage & quad, Importer::TextureAsset* texture);

		GEAR_API void showImage(const glm::vec2 &pos, const float &width, const float &height, Importer::TextureAsset* texture);

		GEAR_API void updateBuffer();
		GEAR_API void draw();
		
		//GEAR_API bool getPicking(MousePos mousePos);

	private:
		std::vector<sScreenImage> quads;
		std::vector<Importer::TextureAsset*> textures;

		std::vector<sScreenImage> bufferQuads;
		std::vector<Importer::TextureAsset*> bufferTextures;

		ShaderProgram* shader;
		GLuint VAO, VBO;
	};
}