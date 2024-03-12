#pragma once
#include "pch.h"
#include "Component.h"
#include "shader.h"
#include "mesh.h"

class MeshRenderer : public Component
{
public :
	MeshRenderer();
	~MeshRenderer();

	unique_ptr<Mesh> mBoxGeo = nullptr;

	ID3DBlob* bmvsByteCode = nullptr;
	ID3DBlob* bmpsByteCode = nullptr;
	vector<D3D12_INPUT_ELEMENT_DESC> vInputLayout;

	void BuildShadersAndInputLayout();
	void InitMesh();
	SubmeshGeometry createSubmesh(array<uint16_t, 36> indices);

private :

	Shader thisShader;
};