#pragma once
#include "pch.h"

class Utils
{
public :

	Utils();
	~Utils();
	UINT CalcConstantBufferByteSize(UINT byteSize);

    ID3D12Resource* CreateDefaultBuffer(ID3D12Device* device,ID3D12GraphicsCommandList* cmdList,void* initData,UINT64 byteSize,ID3D12Resource** uploadBuffer);
};