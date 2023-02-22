#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <DirectXMath.h>
#include "Util.h"
#include "Buffer.h"
#include "Constant.h"
#include "Texture.h"
#include "Entity.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

class Model : public Entity
{
public:
	Model(ID3D12Device* device,const char* fileName, ID3D12GraphicsCommandList* commandList,int sceneIndex);
	Model(const Model& model) = delete;
	Model(Model&& model) = delete;
	Model& operator=(const Model& model) = delete;
	Model& operator=(Model&& model) = delete;

	vector<Vertex> mVertices;
	vector<uint16_t> mIndices;

	obj mObjFeature = { {1.0f,0.0f,0.0f,0.0f,
						0.0f,1.0f,0.0f,0.0f,
						0.0f,0.0f,1.0f,0.0f,
						0.0f,0.0f,0.0f,1.0f},
		{0.7f,0.9f,0.75f},
		0.3f,
		{0.1f,0.1f,0.1f}
	};

	int mVertexBufferSize = 0;
	int mIndexBufferSize = 0;
	int mVertexBufferOffset = 0;
	int mIndexBufferOffset = 0;
	
	int mObjIndex = mNextObjConstantIndex++;

	Texture mTexture;

	static int mNextObjConstantIndex;
};

using Models = unordered_map<string, unique_ptr<Model>>;