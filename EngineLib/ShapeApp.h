#pragma once
#include "pch.h"
#include "Initdx.h"
#include "uploadbuffer.h"
#include "shader.h"
#include "mesh.h"
#include "Utils.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include <vector>

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ConstantBufferData
{
	XMFLOAT4X4 WorldViewProj;
};

class ShapeApp 
{
public :
    ShapeApp();
    ~ShapeApp();
	void Initialize(HWND hWnd);

	vector<GameObject*> listGo;

private :

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSO();

private :

    ID3D12RootSignature* rtRootSignature = nullptr;
    ID3D12DescriptorHeap* mCbvHeap = nullptr;

    UploadBuffer<ConstantBufferData>* mObjectCB = nullptr;


    ID3D12PipelineState* mPSO = nullptr;

    XMFLOAT4X4 mWorld = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
    XMFLOAT4X4 mView = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);;
    XMFLOAT4X4 mProj = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);;

    float fTheta = 1.5f * XM_PI;
    float fPhi = XM_PIDIV4;
    float fRadius = 5.0f;

private :

	float AspectRatio();
	
public:

	void OnResize();
	void Draw(Time* gameTime);
	void Update(Time* gameTime);
	void BuildGeometry();
	thisApp myApp;
	Shader thisShader;
	Utils myUtils;
};