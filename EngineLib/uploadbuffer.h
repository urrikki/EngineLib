#pragma once
#include "pch.h"
#include "Utils.h"

template<typename T>
class UploadBuffer
{
public:

    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
        mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);

        if (isConstantBuffer)
            mElementByteSize = myUtils.CalcConstantBufferByteSize(sizeof(T));

        CD3DX12_HEAP_PROPERTIES HTUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC rsBuffer = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);
        HRESULT hrCCR = (device->CreateCommittedResource(
            &HTUpload,
            D3D12_HEAP_FLAG_NONE,
            &rsBuffer,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mUploadBuffer)));
        assert(SUCCEEDED(hrCCR));

        HRESULT hrupBuffer = (mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
        assert(SUCCEEDED(hrupBuffer));
    }

    
    ~UploadBuffer()
    {
        if (mUploadBuffer != nullptr)
            mUploadBuffer->Unmap(0, nullptr);

        mMappedData = nullptr;
    }

  
    ID3D12Resource* Resource()
    {
        return mUploadBuffer;
    }

    void CopyData(int elementIndex, T& data)
    {
        memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
    }

private:
    ID3D12Resource* mUploadBuffer;
    BYTE* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;

    Utils myUtils;
};

