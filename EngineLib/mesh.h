#pragma once
#include "pch.h"

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	BoundingBox Bounds;
};

struct Mesh
{
	Mesh(); 
	~Mesh();

	string Name;

	ID3DBlob* VertexBufferCPU = nullptr;
	ID3DBlob* IndexBufferCPU = nullptr;

	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferGPU = nullptr;

	ID3D12Resource* VertexBufferUploader = nullptr;
	ID3D12Resource* IndexBufferUploader = nullptr;

	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	unordered_map<string, SubmeshGeometry> DrawArgs;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW IndexBufferView();
	void DisposeUploaders();
};
