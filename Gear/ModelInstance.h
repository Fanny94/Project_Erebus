#pragma once
#include "ModelAsset.h"
#include "Animation.h"
#include <vector>

using namespace Importer;

namespace Gear
{
	class ModelInstance
	{
	public:
		GEAR_API ModelInstance();
		GEAR_API ~ModelInstance();

		GEAR_API void init();
		GEAR_API void allocateBuffer();
		GEAR_API void bindBuffers();
		GEAR_API void bufferData();
		GEAR_API void unloadBuffers();
		 
		GEAR_API void updateWorldMatrices();
		 
		//GEAR_API int addStaticInstance(TransformStruct trans, glm::mat4 world);
		//GEAR_API int addAnimatedInstance(TransformStruct trans, glm::mat4 world, Animation* anim);

		GEAR_API int pushStaticInstance( const TransformStruct& trans, const glm::mat4& world );
		GEAR_API int pushAnimatedInstance( const TransformStruct& trans, const glm::mat4& world, Animation* anim );

		//GEAR_API void popStaticInstance();
		//GEAR_API void popAnimatedInstance();
		GEAR_API void popInstance( int index );
		 
		GEAR_API void incrActiveTransforms();
		GEAR_API void decrActiveTransforms();
		 
		GEAR_API void setAsset(ModelAsset* a);
		GEAR_API void setActiveTransforms(int n);
		GEAR_API void setTransform(int index, TransformStruct trans);
		GEAR_API void setWorldMatrix(int index, glm::mat4 world);
		GEAR_API void setAnimation( int index, Animation* animation );
		GEAR_API void setActive( int index, bool active );
		GEAR_API void setCulled( int index, bool culled );
		 
		GEAR_API ModelAsset* getAsset() const;
		GEAR_API int getWorldMatrixCount() const;
		GEAR_API int getActiveTransforms() const;
		GEAR_API int getAnimationMatrixIndex(int index) const;
		GEAR_API GLuint getVBO() const;
		GEAR_API GLuint getVAO() const;
		GEAR_API TransformStruct* getTransform(int index);
		GEAR_API glm::mat4& getWorldMatrix(int index);
		GEAR_API Animation* getAnimation( int index );
		GEAR_API bool getActive( int index );
		GEAR_API bool getCulled( int index );
		GEAR_API bool getVacant( int index );
		GEAR_API int getOccupied();
		GEAR_API const std::vector<TransformStruct>* getTransforms();
		
	private:
		ModelAsset* asset;
		//std::vector<int> worldIndices;
		std::vector<bool> vacant;
		std::vector<bool> culled;
		std::vector<TransformStruct> transforms;
		int activeTransforms, occupied;
		std::vector<glm::mat4> worldMatrices;
		std::vector<Animation*> animations; // only used for animated instances
		GLuint instanceVBO;
		GLuint instanceVAO;
	};
}