#include "pch.h"
#include "ShapeApp.h"

ShapeApp::ShapeApp() {}

ShapeApp::~ShapeApp() {}

void ShapeApp::Initialize(HWND hWnd)
{
    myApp.Initialize(hWnd);

    BuildDescriptorHeaps();
    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildConstantBuffers();
    BuildPSO();

    myApp.mDirectCmdListAlloc->Reset();
    HRESULT hrResetAlloc = myApp.mCommandList->Reset(myApp.mDirectCmdListAlloc, nullptr);
    assert(SUCCEEDED(hrResetAlloc));

    BuildGeometry();

    HRESULT hrCommandClose = myApp.mCommandList->Close();
    assert(SUCCEEDED(hrCommandClose));
    ID3D12CommandList* cmdsLists[] = { myApp.mCommandList };
    myApp.mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    myApp.FlushCommandQueue();
}

float ShapeApp::AspectRatio()
{
    return static_cast<float>(myApp.iClientWidth) / myApp.iClientHeight;
}

void ShapeApp::OnResize()
{
    myApp.OnResize();

    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * XM_PI, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void ShapeApp::Update(Time* gameTime)
{
    float x = fRadius * sinf(fPhi) * cosf(fTheta);
    float z = fRadius * sinf(fPhi) * sinf(fTheta);
    float y = fRadius * cosf(fPhi);

    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    //XMVECTOR pos = XMVectorSet(0.0f,0.0f, 0.0f, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    XMMATRIX worldViewProj = world * view * proj;

    ConstantBufferData objConstants;
    XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
    mObjectCB->CopyData(0, objConstants);
}

void ShapeApp::Draw(Time* gameTime)
{
    HRESULT hrDcmdAlloc = myApp.mDirectCmdListAlloc->Reset();
    assert(SUCCEEDED(hrDcmdAlloc));
    
    HRESULT hrDcmdAllocCL = myApp.mCommandList->Reset(myApp.mDirectCmdListAlloc, nullptr);
    assert(SUCCEEDED(hrDcmdAllocCL));

    myApp.mCommandList->RSSetViewports(1, &myApp.vpScreen);
    myApp.mCommandList->RSSetScissorRects(1, &myApp.rScissorRect);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(myApp.CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    myApp.mCommandList->ResourceBarrier(1, &barrier);

    myApp.mCommandList->ClearRenderTargetView(myApp.CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    myApp.mCommandList->ClearDepthStencilView(myApp.DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    D3D12_CPU_DESCRIPTOR_HANDLE handleDSV = myApp.DepthStencilView();
    D3D12_CPU_DESCRIPTOR_HANDLE handleCBBV = myApp.CurrentBackBufferView();
    myApp.mCommandList->OMSetRenderTargets(1, &handleCBBV, true, &handleDSV);

    ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap };
    myApp.mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    myApp.mCommandList->SetGraphicsRootSignature(rtRootSignature);

    myApp.mCommandList->SetPipelineState(mPSO);

    D3D12_VERTEX_BUFFER_VIEW boxGeoVBV = mBoxGeo->VertexBufferView();
    myApp.mCommandList->IASetVertexBuffers(0, 1, &boxGeoVBV);
    D3D12_INDEX_BUFFER_VIEW boxGeoIBV = mBoxGeo->IndexBufferView();
    myApp.mCommandList->IASetIndexBuffer(&boxGeoIBV);
    myApp.mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //myApp.mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    myApp.mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

    myApp.mCommandList->DrawIndexedInstanced(
        mBoxGeo->DrawArgs["box"].IndexCount,
        1, 0, 0, 0);

    CD3DX12_RESOURCE_BARRIER barrierTwo = CD3DX12_RESOURCE_BARRIER::Transition(myApp.CurrentBackBuffer(),D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    myApp.mCommandList->ResourceBarrier(1, &barrierTwo);

    HRESULT hrcommandClose = (myApp.mCommandList->Close());

    ID3D12CommandList* cmdsLists[] = { myApp.mCommandList };
    myApp.mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    HRESULT hrSCpresent = (myApp.mSwapChain->Present(0, 0));
    assert(SUCCEEDED(hrSCpresent));
    myApp.iCurrBackBuffer = (myApp.iCurrBackBuffer + 1) % myApp.SwapChainBufferCount;

    myApp.FlushCommandQueue();
}

void ShapeApp::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 100;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask = 0;
    HRESULT hrCreateDesc = (myApp.md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,IID_PPV_ARGS(&mCbvHeap)));
    assert(SUCCEEDED(hrCreateDesc));
}

void ShapeApp::BuildConstantBuffers()
{
    mObjectCB = new UploadBuffer<ConstantBufferData>(myApp.md3dDevice, 1, true);

   // UINT objCBByteSize = myUtils.CalcConstantBufferByteSize(sizeof(mIdentity));
   //
   // D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
   // // Offset to the ith object constant buffer in the buffer.
   // int boxCBufIndex = 0;
   // cbAddress += boxCBufIndex * objCBByteSize;
   //
   // D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
   // cbvDesc.BufferLocation = cbAddress;
   // cbvDesc.SizeInBytes = myUtils.CalcConstantBufferByteSize(sizeof(mIdentity));
   //
   // myApp.md3dDevice->CreateConstantBufferView(
   //     &cbvDesc,
   //     mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ShapeApp::BuildRootSignature()
{
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    //CD3DX12_DESCRIPTOR_RANGE cbvTable;
    //cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    //slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

    slotRootParameter[0].InitAsConstantBufferView(0);

    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,&serializedRootSig, &errorBlob);

    if (errorBlob != nullptr)
    {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    assert(SUCCEEDED(hr));

    HRESULT hrRootS = (myApp.md3dDevice->CreateRootSignature(0,serializedRootSig->GetBufferPointer(),serializedRootSig->GetBufferSize(),IID_PPV_ARGS(&rtRootSignature)));
    assert(SUCCEEDED(hrRootS));
}

void ShapeApp::BuildShadersAndInputLayout()
{
    HRESULT hr = S_OK;

    bmvsByteCode = thisShader.compileShader(L"..\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
    bmpsByteCode = thisShader.compileShader(L"..\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

    vInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

void ShapeApp::BuildGeometry()
{
    std::array<Vertex, 8> vertices =
    {
        Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
        Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
        Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
        Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
        Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
        Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
    };

    std::array<std::uint16_t, 36> indices =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    mBoxGeo = make_unique<Mesh>();
    mBoxGeo->Name = "boxGeo";

    //HRESULT hrCBvertexBuffer = (D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
    //assert(SUCCEEDED(hrCBvertexBuffer));
    //
    //CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
    //
    //HRESULT (D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
    //CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    mBoxGeo->VertexBufferGPU = myUtils.CreateDefaultBuffer(myApp.md3dDevice,
        myApp.mCommandList, vertices.data(), vbByteSize, &mBoxGeo->VertexBufferUploader);

    mBoxGeo->IndexBufferGPU = myUtils.CreateDefaultBuffer(myApp.md3dDevice,
        myApp.mCommandList, indices.data(), ibByteSize, &mBoxGeo->IndexBufferUploader);

    mBoxGeo->VertexByteStride = sizeof(Vertex);
    mBoxGeo->VertexBufferByteSize = vbByteSize;
    mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
    mBoxGeo->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    mBoxGeo->DrawArgs["box"] = submesh;
}

void ShapeApp::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { vInputLayout.data(), (UINT)vInputLayout.size() };
    psoDesc.pRootSignature = rtRootSignature;
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(bmvsByteCode->GetBufferPointer()),
        bmvsByteCode->GetBufferSize()
    };
    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(bmpsByteCode->GetBufferPointer()),
        bmpsByteCode->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = myApp.mBackBufferFormat;
    psoDesc.SampleDesc.Count = myApp.m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = myApp.m4xMsaaState ? (myApp.m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = myApp.formatDepthStencil;
    HRESULT hrCreatGPS = (myApp.md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
    assert(SUCCEEDED(hrCreatGPS));
}