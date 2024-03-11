#pragma once
#include "pch.h"
#include "Initdx.h"
#include "uploadbuffer.h"
#include "shader.h"
#include "mesh.h"
#include "Utils.h"

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct mIdentity
{
	XMFLOAT4X4 WorldViewProj = XMFLOAT4X4 (
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
};

struct ConstantBufferData
{
	XMFLOAT4X4 WorldViewProj;
};


struct MeshGeometry
{
	string Name;

	ID3DBlob* bVertexBufferCPU = nullptr;
	ID3DBlob* bIndexBufferCPU = nullptr;

	ID3D12Resource* bVertexBufferGPU = nullptr;
	ID3D12Resource* bIndexBufferGPU = nullptr;

	ID3D12Resource* bVertexBufferUploader = nullptr;
	ID3D12Resource* bIndexBufferUploader = nullptr;

	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	unordered_map<string, SubmeshGeometry> DrawArgs;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = bVertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView()
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = bIndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	void DisposeUploaders()
	{
		bVertexBufferUploader = nullptr;
		bIndexBufferUploader = nullptr;
	}
};

class ShapeApp 
{
public :
    ShapeApp();
    ~ShapeApp();
	void Initialize(HWND hWnd);

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

    unique_ptr<Mesh> mBoxGeo = nullptr;

    ID3DBlob* bmvsByteCode = nullptr;
    ID3DBlob* bmpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> vInputLayout;

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