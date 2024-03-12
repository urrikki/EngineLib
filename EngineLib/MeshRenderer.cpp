#include "pch.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::BuildShadersAndInputLayout()
{
    bmvsByteCode = thisShader.compileShader(L"..\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
    bmpsByteCode = thisShader.compileShader(L"..\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

    vInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

void MeshRenderer::InitMesh()
{
    mBoxGeo = make_unique<Mesh>();
}

SubmeshGeometry MeshRenderer::createSubmesh(array<uint16_t, 36> indices)
{
    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    return submesh;
}