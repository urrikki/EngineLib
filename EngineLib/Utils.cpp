#include "pch.h"
#include "Utils.h"

Utils::Utils() {}

Utils::~Utils() {}

UINT Utils::CalcConstantBufferByteSize(UINT byteSize) {	
     return (byteSize + 255) & ~255;	
}

ID3D12Resource* Utils::CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, void* initData, UINT64 byteSize, ID3D12Resource** uploadBuffer) {
    ID3D12Resource* defaultBuffer;

    CD3DX12_HEAP_PROPERTIES CHPDefault = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC Bufferbs = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    HRESULT CCR = (device->CreateCommittedResource(
        &CHPDefault,
        D3D12_HEAP_FLAG_NONE,
        &Bufferbs,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&defaultBuffer)));
    assert(SUCCEEDED(CCR));

    CD3DX12_HEAP_PROPERTIES CHPUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC Bufferbs2 = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    HRESULT CCR2(device->CreateCommittedResource(
        &CHPUpload,
        D3D12_HEAP_FLAG_NONE,
        &Bufferbs2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadBuffer)));
    assert(SUCCEEDED(CCR2));

    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    cmdList->ResourceBarrier(1, &barrier);
    UpdateSubresources<1>(cmdList, defaultBuffer, *uploadBuffer, 0, 0, 1, &subResourceData);
    CD3DX12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    cmdList->ResourceBarrier(1, &barrier2);

    return defaultBuffer;
}