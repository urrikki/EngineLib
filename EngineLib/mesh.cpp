#include "pch.h"
#include "mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() {}

D3D12_VERTEX_BUFFER_VIEW Mesh::VertexBufferView() {
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = VertexByteStride;
	vbv.SizeInBytes = VertexBufferByteSize;
	return vbv;
}

D3D12_INDEX_BUFFER_VIEW Mesh::IndexBufferView() {
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;
	return ibv;
}

void Mesh::DisposeUploaders() {
	VertexBufferUploader = nullptr;
	IndexBufferUploader = nullptr;
}