#include "CollisionLayers.h"



CollisionLayers::CollisionLayers(int size)
{
	this->layerMatrixSize = size;
	this->layerMatrix = new bool*[layerMatrixSize];
	this->collisionCheckedMatrix = new bool*[layerMatrixSize];

	for (unsigned int i = 0; i < layerMatrixSize; i++)
	{
		this->layerMatrix[i] = new bool[layerMatrixSize];
		this->collisionCheckedMatrix[i] = new bool[layerMatrixSize];

		for (unsigned int j = 0; j < layerMatrixSize; j++)
		{
			this->layerMatrix[i][j] = true;
		}
	}

	this->resetCollisionCheckedMatrix(); // all collisions are set to false

	this->sphereColliders.resize(layerMatrixSize); // resize to fit the amount of layers we have, fake 2d array
	this->aabbColliders.resize(layerMatrixSize);
}


CollisionLayers::~CollisionLayers()
{
	for (unsigned int i = 0; i < layerMatrixSize; i++)
	{
		delete[] layerMatrix[i];
		delete[] collisionCheckedMatrix[i];
	}
	delete[] layerMatrix;
	delete[] collisionCheckedMatrix;
}

void CollisionLayers::addHitbox(AABBCollider * aabbCollider) 
{
	this->aabbColliders[0].push_back(aabbCollider); // default layer is 0
}

void CollisionLayers::addHitbox(SphereCollider * sphereCollider)
{
	this->sphereColliders[0].push_back(sphereCollider);
}

void CollisionLayers::addHitbox(AABBCollider * aabbCollider, unsigned int layer)
{
	this->aabbColliders[layer].push_back(aabbCollider);
}

void CollisionLayers::addHitbox(SphereCollider * sphereCollider, unsigned int layer)
{
	this->sphereColliders[layer].push_back(sphereCollider);
}

std::vector<SphereCollider*>* CollisionLayers::getSphereColliders(int layer)
{
	std::vector<SphereCollider*>* colliders = nullptr;

	colliders = &this->sphereColliders[layer];

	return colliders;
}

std::vector<AABBCollider*>* CollisionLayers::getAABBColliders(int layer)
{
	std::vector<AABBCollider*>* colliders = nullptr;

	colliders = &this->aabbColliders[layer];

	return colliders;
}

bool CollisionLayers::isLayerChecked(int layer1, int layer2)
{
	return this->collisionCheckedMatrix[layer1][layer2];
}

void CollisionLayers::checkLayer(int layer1, int layer2)
{
	this->collisionCheckedMatrix[layer1][layer2] = true;
	this->collisionCheckedMatrix[layer2][layer1] = true;
}

void CollisionLayers::resetCollisionCheckedMatrix() // set all old collisions to false
{
	for (unsigned int i = 0; i < layerMatrixSize; i++)
	{
		for (unsigned int j = 0; j < layerMatrixSize; j++)
		{
			this->collisionCheckedMatrix[i][j] = false;
		}
	}
}

std::vector<int> CollisionLayers::getLayerCollisions(int layer)
{
	std::vector<int> layerCollisions;

	for (unsigned int i = 0; i < this->layerMatrixSize; i++)
	{
		bool derrr = false;
		derrr = this->layerMatrix[layer][i];
		if (this->layerMatrix[layer][i] == true) // if the layermatrix says that these two layers should collide add it to vector
		{
			layerCollisions.push_back(i);
		}
		
	}

	return layerCollisions;
}

std::vector<int> CollisionLayers::getUncheckedLayerCollisions(int layer)
{
	std::vector<int> layerCollisions;

	for (unsigned int i = 0; i < this->layerMatrixSize; i++)
	{
		bool derrr = false;
		derrr = this->layerMatrix[layer][i];
		derrr = this->collisionCheckedMatrix[layer][i];
		if (this->layerMatrix[layer][i] == true && this->collisionCheckedMatrix[layer][i] == false) // if the layermatrix says that these two layers should collide AND it is not checked add it to vector
		{
			layerCollisions.push_back(i);
		}

	}

	return layerCollisions;
}

unsigned int CollisionLayers::getLayerMatrixSize()
{
	return this->layerMatrixSize;
}
