#pragma once
#include "pch.h"
#include "Time.h"
#include "synchapi.h"

class thisApp
{
public :

	thisApp();
	virtual ~thisApp();
	void Initialize();
	void createDevice();
	void createFence();
	void createMSAAQuality();
	void CommandSystem();
	void CreateSwapChain();
	void FlushCommandQueue();
	void CreateRtvAndDsvDescriptorHeaps();
	void CalculateFrame(HWND mainWin, wstring mMainWndCaption);
	int mClientWidth = 800;
	int mClientHeight = 600;

	Time thisTime;

private :

	IDXGIFactory4* mdxgiFactory;
	ID3D12Device* md3dDevice;
	ID3D12Fence* mFence;

	int mRtvDescriptorSize = 0;
	int mDsvDescriptorSize = 0;
	int mCbvSrvUavDescriptorSize = 0;

	int m4xMsaaQuality = 0;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	ID3D12CommandQueue* mCommandQueue;
	ID3D12CommandAllocator* mDirectCmdListAlloc;
	ID3D12GraphicsCommandList* mCommandList;

	IDXGISwapChain* mSwapChain;
	bool m4xMsaaState = false;

	//const car on veux juste swap entre les deux 
	//static 
	static const int SwapChainBufferCount = 2;
	HWND  mhMainWnd = nullptr;

	int mCurrentFence = 0;
	ID3D12DescriptorHeap* mRtvHeap;
	ID3D12DescriptorHeap* mDsvHeap;
};