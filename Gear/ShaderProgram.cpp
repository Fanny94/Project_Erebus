#include "ShaderProgram.h"
#include "ShaderCreator.h"

ShaderProgram::ShaderProgram() :programID(0), shaderIDs(nullptr), textureIDs(nullptr), nrOfShaders(0), nrOfTextures(0), totalAttributes(0), framebufferID(0), nrOfUniforms(0)
{

}

ShaderProgram::ShaderProgram(int nrShaders, std::string* paths, GLuint* shaderTypes) : ShaderProgram()
{

	nrOfShaders = nrShaders;
	shaderIDs = new GLuint[nrOfShaders];
	programID = createShaderProgram();

	std::string* shaderContent = new std::string[nrOfShaders];
	for (int i = 0; i < nrShaders; i++)
	{
		shaderContent[i] = readShader(paths[i]);
		shaderIDs[i] = createShader(shaderTypes[i], shaderContent[i], programID, totalAttributes);
		glAttachShader(programID, shaderIDs[i]);
	}
	glLinkProgram(programID);

	for (int i = 0; i < nrShaders; i++)
	{
		glDetachShader(programID, shaderIDs[i]);
		glDeleteShader(shaderIDs[i]);
	}
	delete[] shaderContent;
}

ShaderProgram::ShaderProgram(shaderBaseType type, std::string shaderName) : ShaderProgram() {

	this->shaderName = shaderName;
	nrOfShaders = getNumShaders(type);
	std::string* paths = getPaths(type, shaderName);
	GLuint* shaderTypes = getTypes(type);

	shaderIDs = new GLuint[nrOfShaders];
	programID = createShaderProgram();
	std::string* shaderContent = new std::string[nrOfShaders];
	for (int i = 0; i < nrOfShaders; i++)
	{
		shaderContent[i] = readShader(paths[i]);
		shaderIDs[i] = createShader(shaderTypes[i], shaderContent[i], programID, totalAttributes);
		glAttachShader(programID, shaderIDs[i]);
	}
	glLinkProgram(programID);

	for (int i = 0; i < nrOfShaders; i++)
	{
		glDetachShader(programID, shaderIDs[i]);
		glDeleteShader(shaderIDs[i]);
	}
	for (int i = 0; i < nrOfShaders; i++)
	{
		addAllUniforms(shaderContent[i]);
	}

	delete[] shaderContent;
	delete[] paths;
	delete[] shaderTypes;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(programID);
	if (textureIDs != nullptr)
		delete textureIDs;
	if (shaderIDs != nullptr)
		delete shaderIDs;
}

void ShaderProgram::framebufferInit(int nrTex, int width, int height, GLuint* internalFormat, GLuint* format, GLuint* type, GLuint* attachments)
{
	nrOfTextures = nrTex;
	textureIDs = new GLuint[nrOfTextures];
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	for (int i = 0; i < nrOfTextures; i++)
	{
		textureIDs[i] = addTexture(width, height, internalFormat[i], format[i], type[i]);
	}
	bindFramebuffer(nrOfTextures, attachments, textureIDs, framebufferID);
}

void ShaderProgram::deferredInit(int nrTex, int width, int height, GLuint * internalFormat, GLuint * format, GLenum * type, GLuint * attachments)
{
	nrOfTextures = nrTex;
	textureIDs = new GLuint[nrOfTextures];
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	GLuint gPosition, gNormal, gColorSpec;

	// - Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[0], width, height, 0, format[0], GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	this->textureIDs[0] = gPosition;

	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[1], width, height, 0, format[1], GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	this->textureIDs[1] = gNormal;

	// - Color + Specular color buffer
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[2], width, height, 0, format[2], GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	this->textureIDs[2] = gColorSpec;

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachment[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachment);

	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void ShaderProgram::initFramebuffer(int nrTex, int width, int height, GLfloat* filter, GLenum* internalFormat, GLenum* format, GLenum* type, GLenum* attachments, bool clamp)
{
	nrOfTextures = nrTex;
	this->width = width;
	this->height = height;

	framebufferID = 0;
	renderBuffer = 0;

	textureIDs = new GLuint[nrOfTextures];
	InitTextures(filter, internalFormat, format, type, clamp);
	InitRenderTargets(attachments);
}

void ShaderProgram::initFramebuffer(int nrTex, int width, int height, GLfloat filter, GLenum internalFormat, GLenum format, GLenum type, GLenum attachments, bool clamp)
{
	nrOfTextures = nrTex;
	this->width = width;
	this->height = height;

	framebufferID = 0;
	renderBuffer = 0;

	textureIDs = new GLuint[nrOfTextures];
	InitTextures(&filter, &internalFormat, &format, &type, clamp);
	InitRenderTargets(&attachments);
}

void ShaderProgram::bindTexToLocation(GLuint* textures)
{
	for (int i = 0; i < nrOfUniforms; i++)
	{

	}
}

void ShaderProgram::BindTexturesToProgram(ShaderProgram *shader, const char *name, GLuint textureLoc, GLuint textureid)
{
	glActiveTexture(GL_TEXTURE0 + textureLoc);
	glUniform1i(shader->getUniformLocation(name), textureLoc);
	glBindTexture(GL_TEXTURE_2D, textureIDs[textureid]);
}

GLuint ShaderProgram::getProgramID()
{
	return programID;
}

GLuint* ShaderProgram::getTextures()
{
	return textureIDs;
}

GLuint ShaderProgram::getFramebufferID()
{
	return framebufferID;
}

void ShaderProgram::addUniform(std::string uniform)
{
	int uniformLocation = glGetUniformLocation(programID, uniform.c_str());

	if (uniformLocation == 0xFFFFFFFF)
	{
		printf("Error in shader %s : Could not find uniform: %s\n", shaderName.c_str(), uniform.c_str());
		return;
	}

	uniforms.insert(std::pair<std::string, int>(uniform, uniformLocation));
}

void ShaderProgram::addAllUniforms(std::string shaderText)
{
	const static std::string UNIFORM_KEYWORD = "uniform";
	int uniformStartLocation = shaderText.find(UNIFORM_KEYWORD);
	while (uniformStartLocation > -1)
	{
		int begin = uniformStartLocation + UNIFORM_KEYWORD.length() + 1;
		int end = shaderText.find(";", begin);

		std::string uniformLine = shaderText.substr(begin, (end-begin));
		int tempEnd = uniformLine.find("[");
		if (tempEnd > -1)
		{
			uniformLine = uniformLine.substr(0, (tempEnd));
		}

		std::string uniformType = uniformLine.substr(0, (uniformLine.find(" ")));
		std::string uniformName = uniformLine.substr(uniformLine.find(" ") + 1, uniformLine.length() - uniformType.length() - 1);

		addUniform(uniformName);

		uniformStartLocation = shaderText.find(UNIFORM_KEYWORD, uniformStartLocation + UNIFORM_KEYWORD.length());
	}
}

GLuint ShaderProgram::getUniformLocation(std::string pos)
{
	return uniforms.at(pos);
}

void ShaderProgram::setUniform(glm::mat4 &matrix4x4, std::string position, int count)
{
	glUniformMatrix4fv(uniforms.at(position), count, GL_FALSE, glm::value_ptr(matrix4x4));
}

void ShaderProgram::setUniform(glm::vec3 &vec3, std::string position, int count)
{
	glUniform3fv(uniforms.at(position), count, glm::value_ptr(vec3));
}
void ShaderProgram::setUniform4fv(glm::mat4 *matrix4x4, std::string position, int count)
{
	glUniformMatrix4fv(uniforms.at(position), count, GL_FALSE, &matrix4x4[0][0][0]);
}

void ShaderProgram::setUniform4cfv(const GLfloat *matrix4x4, std::string position, int count)
{
	glUniformMatrix4fv(uniforms.at(position), count, GL_FALSE, matrix4x4);
}

void ShaderProgram::setUniform1fv(float & vec1, std::string position, int count)
{
	glUniform1fv(uniforms.at(position), count, &vec1);
}
void ShaderProgram::setUniform(float &floatValue, std::string position)
{
	glUniform1f(uniforms.at(position), floatValue);
}
void ShaderProgram::setUniform(int intValue, std::string position)
{
	glUniform1i(uniforms.at(position), intValue);
}

std::string* ShaderProgram::getPaths(const shaderBaseType& type, const std::string& path) {
	std::string* returnVal;
	switch (type)
	{
	case shaderBaseType::VERTEX_FRAGMENT:
		returnVal = new std::string[2];
		returnVal[0] = baseFolder + path + vertEnd;
		returnVal[1] = baseFolder + path + fragEnd;
		break;
	case shaderBaseType::VERTEX_GEOMETRY_FRAGMENT:
		returnVal = new std::string[3];
		returnVal[0] = baseFolder + path + vertEnd;
		returnVal[1] = baseFolder + path + fragEnd;
		returnVal[2] = baseFolder + path + geomEnd;
		break;
	case shaderBaseType::COMPUTESHADER:
		returnVal = new std::string[1];
		returnVal[0] = baseFolder + path + compEnd;
		break;
	default:
		break;
	}
	return returnVal;
}

GLuint* ShaderProgram::getTypes(const shaderBaseType& type) {
	GLuint* returnVal;

	/*
	types[0] = GL_VERTEX_SHADER;
	types[1] = GL_FRAGMENT_SHADER;
	types[2] = GL_GEOMETRY_SHADER;
	*/
	switch (type)
	{
	case shaderBaseType::VERTEX_FRAGMENT:
		returnVal = new GLuint[2];
		returnVal[0] = GL_VERTEX_SHADER;
		returnVal[1] = GL_FRAGMENT_SHADER;
		break;
	case shaderBaseType::VERTEX_GEOMETRY_FRAGMENT:
		returnVal = new GLuint[3];
		returnVal[0] = GL_VERTEX_SHADER;
		returnVal[1] = GL_FRAGMENT_SHADER;
		returnVal[2] = GL_GEOMETRY_SHADER;
		break;
	case shaderBaseType::COMPUTESHADER:
		returnVal = new GLuint[1];
		returnVal[0] = GL_COMPUTE_SHADER;
		break;
	default:
		break;
	}

	return returnVal;
}

int ShaderProgram::getNumShaders(const shaderBaseType& type) {
	int returnValue = 0;
	switch (type)
	{
	case shaderBaseType::VERTEX_FRAGMENT:
		returnValue = 2;
		break;
	case shaderBaseType::VERTEX_GEOMETRY_FRAGMENT:
		returnValue = 3;
		break;
	case shaderBaseType::COMPUTESHADER:
		returnValue = 1;
		break;
	default:
		break;
	}

	return returnValue;
}

void ShaderProgram::InitTextures(GLfloat * filter, GLenum * internalFormat, GLenum * format, GLenum * type, bool clamp)
{
	for (int i = 0; i < nrOfTextures; i++)
	{
		glGenTextures(1, &textureIDs[i]);
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter[i]);

		if (clamp)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[i], width, height, 0, format[i], type[i], NULL);
	}
}

void ShaderProgram::InitRenderTargets(GLenum * attachments)
{
	if (attachments == 0)
		return;

	GLenum drawBuffers[32];      //32 is the max number of bound textures in OpenGL
	assert(nrOfTextures <= 32); //Assert to be sure no buffer overrun should occur

	bool hasDepth = false;
	for (int i = 0; i < nrOfTextures; i++)
	{
		if (attachments[i] == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
			drawBuffers[i] = attachments[i];

		if (attachments[i] == GL_NONE)
			continue;

		if (framebufferID == 0)
		{
			glGenFramebuffers(1, &framebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, textureIDs[i], 0);
	}

	if (framebufferID == 0)
		return;

	if (!hasDepth)
	{
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	}

	glDrawBuffers(nrOfTextures, drawBuffers);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer creation failed!" << std::endl;
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
