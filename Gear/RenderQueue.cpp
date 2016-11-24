#include "RenderQueue.h"

RenderQueue::RenderQueue(): nrOfWorlds(0), totalWorlds(0), worldMatrices(nullptr)
{
	for (size_t i = 0; i < ShaderType::NUM_SHADER_TYPES; i++)
	{
		allShaders[i] = nullptr;
	}
}

RenderQueue::~RenderQueue()
{
	if (worldMatrices != nullptr)
		delete[] worldMatrices;
	for (size_t i = 0; i < ShaderType::NUM_SHADER_TYPES; i++)
		if (allShaders[i] != nullptr)
			delete allShaders[i];
}

void RenderQueue::init()
{
	std::string paths[3];
	paths[0] = "Shaders/forward.vert";
	paths[1] = "Shaders/forward.frag";
	paths[2] = "Shaders/forward.geom";
	GLuint types[3];

	types[0] = GL_VERTEX_SHADER;
	types[1] = GL_FRAGMENT_SHADER;
	types[2] = GL_GEOMETRY_SHADER;
	
	allShaders[ShaderType::FORWARD] = new ShaderProgram(shaderBaseType::VERTEX_GEOMETRY_FRAGMENT, "forward");
}

void RenderQueue::updateUniforms(Camera* camera)
{
	allShaders[FORWARD]->use();
	allShaders[FORWARD]->addUniform(camera->getProjectionMatrix(), "projectionMatrix");
	allShaders[FORWARD]->addUniform(camera->getViewMatrix(), "viewMatrix");
	allShaders[FORWARD]->addUniform(camera->getPosition(), "viewPos");
	allShaders[FORWARD]->addUniform(camera->getPosition(), "lightPos");
	allShaders[FORWARD]->addUniform(glm::vec3(1.0f, 1.0f, 1.0f), "lightColor");
	allShaders[FORWARD]->unUse();
}

void RenderQueue::configure(RenderQueueId &id, GLuint &shaderProgramId)
{

	if (currentShader != id.shaderProgram)
	{
		allShaders[currentShader]->unUse();
		allShaders[id.shaderProgram]->use();
		shaderProgramId = allShaders[id.shaderProgram]->getProgramID();
		currentShader = id.shaderProgram;
	}

	if (currentTexture != id.texture)
	{
		//use id.texture
		currentTexture = id.texture;
	}
}

void RenderQueue::process(std::vector<RenderQueueElement*> &elements)
{
	size_t size = elements.size();

	std::vector<size_t> indices(size);
	std::iota(indices.begin(), indices.end(), 0);

	std::sort(indices.begin(), indices.end(),
		[&elements](size_t i1, size_t i2) {return elements[i2] < elements[i1]; });

	currentShader = 0;
	GLuint shaderProgramId = allShaders[currentShader]->getProgramID();
	allShaders[currentShader]->use();

	for (int i = 0; i < size; i++)
	{
		RenderQueueElement* el = elements[indices[i]];
		configure(el->id, shaderProgramId);
		el->draw(shaderProgramId);
	}
	allShaders[currentShader]->unUse();
}

GEAR_API void RenderQueue::allocateWorlds(int n)
{
	worldMatrices = new glm::mat4[n];
}

GEAR_API void RenderQueue::draw()
{
	currentShader = FORWARD;
	allShaders[currentShader]->use();
	GLuint worldMatrixLocation = glGetUniformLocation(this->allShaders[currentShader]->getProgramID() , "worldMatrix");
	for (int i = 0; i < allModels.size(); i++)
	{	
		Importer::ModelAsset* modelAsset = allModels[i]->getModelAsset();
		int meshes = modelAsset->getHeader()->meshCount;
		for (int k = 0; k < allModels[i]->matrixIndices.size(); k++)
		{
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrices[allModels[i]->matrixIndices[k]][0][0]);			
			for (int j = 0; j < meshes; j++)
			{
				glBindBuffer(GL_ARRAY_BUFFER, modelAsset->getVertexBuffer(j));
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Importer::sVertex), 0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Importer::sVertex), (void*)(sizeof(float) * 3));
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Importer::sVertex), (void*)(sizeof(float) * 6));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelAsset->getIndexBuffer(j));
				glDrawElements(GL_TRIANGLES, modelAsset->getBufferSize(j), GL_UNSIGNED_INT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
	}
	allShaders[currentShader]->unUse();
}

GEAR_API void RenderQueue::update(float * pos, int * indices, int n)
{
	for (int i = 0; i < n; i++)
	{
		worldMatrices[i][3][0] = pos[i*3];
		worldMatrices[i][3][1] = pos[i * 3 + 1];
		worldMatrices[i][3][2] = pos[i * 3 + 2];
	}
}

int RenderQueue::modelAdded(Model* model)
{
	allModels.push_back(model);
	worldMatrices[nrOfWorlds] = glm::mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, nrOfWorlds, 1);
	return nrOfWorlds++;
}
