#pragma once
#include "pch.h"
#include "Time.h"
#include "synchapi.h"

class thisApp
{
public :

	thisApp();
	virtual ~thisApp();
	void Initialize(HWND hWnd);
	void createDevice();
	void createFence();
	void createMSAAQuality();
	void CommandSystem();
	void CreateSwapChain();
	void FlushCommandQueue();
	void CreateRtvAndDsvDescriptorHeaps();
	void CalculateFrame(HWND mainWin, wstring mMainWndCaption);
	void OnResize();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView();

	void Draw(Time* gameTime);
	void Update(Time* gameTime);

private :

	IDXGIFactory4* mdxgiFactory;
	ID3D12Device* md3dDevice;
	ID3D12Fence* mFence;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

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
	HWND  mhMainWnd = NULL;

	int mCurrentFence = 0;
	ID3D12DescriptorHeap* mRtvHeap;
	ID3D12DescriptorHeap* mDsvHeap;

	D3D12_VIEWPORT vpScreenViewport;
	D3D12_RECT rScissorRect;

	DXGI_FORMAT formatDepthStencil = DXGI_FORMAT_D24_UNORM_S8_UINT;
	D3D12_VIEWPORT vpScreen;

public :

	ID3D12Resource* CurrentBackBuffer();
	ID3D12Resource* tSwapChainBuffer[SwapChainBufferCount];
	ID3D12Resource* tDepthStencilBuffer;
	int iCurrBackBuffer = 0;

	int iClientWidth = 800;
	int iClientHeight = 600;
	float fTimeElapsed = 0.0f;

	Time thisTime;
};