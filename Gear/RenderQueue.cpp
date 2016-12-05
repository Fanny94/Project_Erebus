#include "RenderQueue.h"


RenderQueue::RenderQueue() : nrOfWorlds(0), totalWorlds(0), worldMatrices(nullptr)
{
	for (size_t i = 0; i < ShaderType::NUM_SHADER_TYPES; i++)
	{
		allShaders[i] = nullptr;
	}

	int maximumNumberOfInstancesPerModel = 105;
	tempMatrices = new glm::mat4[maximumNumberOfInstancesPerModel];

}

RenderQueue::~RenderQueue()
{
	if (worldMatrices != nullptr)
		delete[] worldMatrices;
	for (size_t i = 0; i < ShaderType::NUM_SHADER_TYPES; i++)
		if (allShaders[i] != nullptr)
			delete allShaders[i];
	delete[] tempMatrices;
}

void RenderQueue::init()
{
	allShaders[ShaderType::FORWARD] = new ShaderProgram(shaderBaseType::VERTEX_GEOMETRY_FRAGMENT, "forward");
	allShaders[ShaderType::PARTICLES] = new ShaderProgram(shaderBaseType::VERTEX_GEOMETRY_FRAGMENT, "particle");
	allShaders[ShaderType::HEIGHTMAP] = new ShaderProgram(shaderBaseType::VERTEX_FRAGMENT, "heightmap");
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

	allShaders[PARTICLES]->use();
	allShaders[PARTICLES]->addUniform(camera->getProjectionMatrix(), "projectionMatrix");
	allShaders[PARTICLES]->addUniform(camera->getViewMatrix(), "viewMatrix");
	allShaders[PARTICLES]->unUse();
	
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
	if (worldMatrices)
		delete[] worldMatrices;
	worldMatrices = new glm::mat4[n];
}

GEAR_API void RenderQueue::draw(std::vector<ModelInstance>* instances)
{
	currentShader = FORWARD;
	allShaders[currentShader]->use();
	GLuint worldMatrixLocation = glGetUniformLocation(this->allShaders[currentShader]->getProgramID(), "worldMatrix");
	GLuint worldMatricesLocation = glGetUniformLocation(allShaders[currentShader]->getProgramID(), "worldMatrices");

	for( int i=0; i<instances->size(); i++ )
	{
		ModelAsset* modelAsset = instances->at(i).asset;
		int meshes = modelAsset->getHeader()->numMeshes;
		int numInstance = 0;
		for( int j=0; j< instances->at(i).worldIndices.size(); j++ )
		{
			/*glUniformMatrix4fv( worldMatrixLocation, 1, GL_FALSE, &worldMatrices[instances[i].worldIndices[k]][0][0] );
			for (int j = 0; j < meshes; j++)
			{
			int vertexSize = (modelAsset->getMesh(j)->numAnimVertices > 0 ? sizeof(Importer::sSkeletonVertex) : sizeof(Importer::sVertex));

			glBindBuffer(GL_ARRAY_BUFFER, modelAsset->getVertexBuffer(j));
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float) * 6));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelAsset->getIndexBuffer(j));
			glDrawElements(GL_TRIANGLES, modelAsset->getBufferSize(j), GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			}*/

			tempMatrices[numInstance++] = worldMatrices[instances->at(i).worldIndices[j]];
		}

		glUniformMatrix4fv(worldMatricesLocation, numInstance, GL_FALSE, &tempMatrices[0][0][0]);

		for (int j = 0; j<modelAsset->getHeader()->numMeshes; j++)
		{
			//0 == STATIC 1 == DYNAMIC/ANIMATEDS
			int aids = modelAsset->getHeader()->TYPE == 0 ? sizeof(Importer::sVertex) : sizeof(Importer::sSkeletonVertex);
			glBindBuffer(GL_ARRAY_BUFFER, modelAsset->getVertexBuffer(j));
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, aids, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, aids, (void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, aids, (void*)(sizeof(float) * 6));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelAsset->getIndexBuffer(j));
			glDrawElementsInstanced(GL_TRIANGLES, modelAsset->getBufferSize(j), GL_UNSIGNED_INT, 0, numInstance);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	allShaders[currentShader]->unUse();

	allShaders[PARTICLES]->use();

	for (size_t i = 0; i <  particleSystem.size(); i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			GLuint loc = glGetUniformLocation(allShaders[PARTICLES]->getProgramID(), "particleSize");

			if (loc != -1)
			{
				glUniform1f(loc, 2.0f);
			}

			/*glBindBuffer(GL_ARRAY_BUFFER, particles[i]->particleVertexBuffer);*/
			glBindBuffer(GL_ARRAY_BUFFER, particleSystem[i]->particles[j]->particleVertexBuffer);

			glm::vec3 partArray[2] = { particleSystem[i]->particles[j]->getPosition(), particleSystem[i]->particles[j]->getColor() };

			glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * 2) * maxParticles, partArray, GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, partArray, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)sizeof(glm::vec3));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glDrawArraysInstanced(GL_POINTS, 0, 10, maxParticles);
			//glDrawArrays(GL_POINTS, 0, 1);
		}
	}

	allShaders[PARTICLES]->unUse();
}

GEAR_API void RenderQueue::update(float * pos, int * indices, int n, glm::vec3* lookAts)
{
	glm::vec3 tempLook;
	for (int i = 0; i < n; i++)
	{
	int index = i * 6;

	int rotIndexY = index + 4;
	glm::mat4 rotationY = glm::rotate(glm::mat4(), pos[index + 4], { 0, 1, 0 });
		
	int rotIndexZ = index + 5;
	tempLook = glm::normalize(glm::vec3(lookAts[i].x, 0, lookAts[i].z));
	glm::vec3 axis = glm::cross(tempLook, { 0, 1, 0 });
	glm::mat4 rotationZ = glm::rotate(glm::mat4(), pos[index + 5], axis);

	worldMatrices[i] = rotationZ * rotationY;
	worldMatrices[i][3][0] = pos[index];
	worldMatrices[i][3][1] = pos[index + 1];
	worldMatrices[i][3][2] = pos[index + 2];
	}
}

int RenderQueue::addModelInstance(ModelAsset* asset)
{
	int result = this->nrOfWorlds++;

	int index = -1;
	for (int i = 0; i < instances.size() && index < 0; i++)
		if (instances[i].asset == asset)
			index = i;

	if (index < 0)
	{
		ModelInstance instance;
		instance.asset = asset;
		instance.worldIndices.push_back(result);

		index = instances.size();
		instances.push_back(instance);
	}

	instances[index].worldIndices.push_back(result);
	worldMatrices[result] = glm::mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, nrOfWorlds, 1);

	return result;
}

ShaderProgram* RenderQueue::getShaderProgram(ShaderType type) {
	return this->allShaders[type];
}

GEAR_API int RenderQueue::generateWorldMatrix()
{
	int result = nrOfWorlds++;
	worldMatrices[result];
	return result;
}