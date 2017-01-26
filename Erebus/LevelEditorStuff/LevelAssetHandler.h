#pragma once

#include "BaseIncludes.h"
#include "Assets.h"
#include "ModelAsset.h"
#include "HeightMap.h"
#include <vector>
#include <string>
#include "AntTweakBar.h"
#include "LevelActorHandler.h"

class LevelAssetHandler
{
public:
	//LevelAssetHandler( Importer::Assets* assets );
	~LevelAssetHandler();

	static LevelAssetHandler* getInstance();
	static void deleteInstance();
	static void resetInstance();

	void load();
	void updateBars();
	void updateAssetsBar();
	//void showContextBar( bool visible );
	void selectModel( std::string model );
	void selectTexture( std::string texture );
	//void selectPrefab( std::string prefab );
	void showContextBar( std::string asset );
	void hideContextBar();
	void addToActor();
	void onMouseReleased();

	void selectPrefab( int index );
	int getSelectedPrefabIndex();
	const std::string& getSelectedPrefab();

	void setAssets( Importer::Assets* assets );
	void setInputs( Inputs* inputs );
	void setTweakBars( TweakBar* assetsBar, TweakBar* contextBar );

	Importer::Assets* getAssets();

private:
	LevelAssetHandler();

	void loadAssets( std::vector<std::string>* container, std::string folder, std::string filter = "*" );
	static void TW_CALL onSetPrefab( const void* value, void* clientData );
	static void TW_CALL onGetPrefab( void* value, void* clientData );
	static void TW_CALL onSelectModel( void* args );
	static void TW_CALL onSelectTexture( void* args );
	static void TW_CALL onAdd( void* args );
	static void TW_CALL onClose( void* args );

	Importer::Assets* assets;

	std::vector<std::string> prefabs;
	std::vector<bool> prefabSelection;
	std::vector<int> prefabSelectionIndices;
	std::vector<std::string> models;
	std::vector<std::string> textures;

	TweakBar* assetsBar, *contextBar;
	bool contextBarVisible;
	Inputs* inputs;
	//std::string selectedModel;
	std::string selectedAsset;
	bool modelAsset;
	int selectedPrefab;

	static LevelAssetHandler* g_instance;
};