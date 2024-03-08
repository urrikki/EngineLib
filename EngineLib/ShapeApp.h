#pragma once
#include "pch.h"
#include "Initdx.h"

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;
	BoundingBox Bounds;
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
    virtual void BuildGeometry() ;

private :

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSO();

private :

    ID3D12RootSignature* rtRootSignature = nullptr;
    ID3D12DescriptorHeap* mCbvHeap = nullptr;

    //std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

    ID3DBlob* bmvsByteCode = nullptr;
    ID3DBlob* bmpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> vInputLayout;

    ID3D12PipelineState* mPSO = nullptr;

	XMFLOAT4X4 mIdentity;
    XMFLOAT4X4 mWorld = mIdentity;
    XMFLOAT4X4 mView = mIdentity;
    XMFLOAT4X4 mProj = mIdentity;

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

	thisApp myApp;
};