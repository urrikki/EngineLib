#include "pch.h"
#include "Initdx.h"
#include <iostream>

thisApp::thisApp() {}

thisApp::~thisApp() {}

void thisApp::Initialize(HWND hWnd) {

mhMainWnd = hWnd;
#if defined(DEBUG) || defined(_DEBUG) 
	{
		ID3D12Debug* debugController;
		HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
		assert(SUCCEEDED(hr));
		debugController->EnableDebugLayer();
	}
#endif

	createDevice();
	createFence();

	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	createMSAAQuality();
	CommandSystem();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();

	thisTime.Start();
}

void thisApp::OnResize()
{
	//assert(md3dDevice);
	//assert(mSwapChain);
	//assert(mDirectCmdListAlloc);

	FlushCommandQueue();

	HRESULT hrmCommandLReset = mCommandList->Reset(mDirectCmdListAlloc, nullptr);
	assert(SUCCEEDED(hrmCommandLReset));

	/*for (int i = 0; i < SwapChainBufferCount; ++i)
		tSwapChainBuffer[i]->Reset();
	tDepthStencilBuffer->Reset();*/

	
	HRESULT hrResizeSC = (mSwapChain->ResizeBuffers(SwapChainBufferCount,iClientWidth, iClientHeight,mBackBufferFormat,DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
	assert(SUCCEEDED(hrResizeSC));

	iCurrBackBuffer = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		HRESULT hrGetBuffer = (mSwapChain->GetBuffer(i, IID_PPV_ARGS(&tSwapChainBuffer[i])));
		assert(SUCCEEDED(hrGetBuffer));
		md3dDevice->CreateRenderTargetView(tSwapChainBuffer[i], nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = iClientWidth;
	depthStencilDesc.Height = iClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = formatDepthStencil;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	CD3DX12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	HRESULT hwCreateResource = (md3dDevice->CreateCommittedResource(&properties,D3D12_HEAP_FLAG_NONE,&depthStencilDesc,D3D12_RESOURCE_STATE_COMMON,&optClear,IID_PPV_ARGS(&tDepthStencilBuffer)));
	assert(SUCCEEDED(hwCreateResource));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = formatDepthStencil;
	dsvDesc.Texture2D.MipSlice = 0;
	md3dDevice->CreateDepthStencilView(tDepthStencilBuffer, &dsvDesc, DepthStencilView());

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(tDepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	mCommandList->ResourceBarrier(1, &barrier);

	HRESULT hrClose = (mCommandList->Close());
	assert(SUCCEEDED(hrClose));
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	vpScreen.TopLeftX = 0;
	vpScreen.TopLeftY = 0;
	vpScreen.Width = static_cast<float>(iClientWidth);
	vpScreen.Height = static_cast<float>(iClientHeight);
	vpScreen.MinDepth = 0.0f;
	vpScreen.MaxDepth = 1.0f;

	rScissorRect = { 0, 0, iClientWidth, iClientHeight };
}

void thisApp::createDevice() {
	HRESULT hrFactory = (CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));
	assert(SUCCEEDED(hrFactory));

	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice));
}

void thisApp::createFence() {
	HRESULT hrFence = (md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	assert(SUCCEEDED(hrFence));
}

void thisApp::createMSAAQuality() {

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	HRESULT hrFeature(md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));
	assert(SUCCEEDED(hrFeature));
	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

}

void thisApp::CommandSystem()
{
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	HRESULT hrCommandQueue(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	assert(SUCCEEDED(hrCommandQueue));
	HRESULT hrCommandAllocator(md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mDirectCmdListAlloc)));
	assert(SUCCEEDED(hrCommandAllocator));
	HRESULT hrCommandList(md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mDirectCmdListAlloc, nullptr, IID_PPV_ARGS(&mCommandList)));
	assert(SUCCEEDED(hrCommandList));
	mCommandList->Close();
}

void thisApp::CreateSwapChain() {
	// Release the previous swapchain we will be recreating.
	//mSwapChain->reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = iClientWidth;
	sd.BufferDesc.Height = iClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hrSwapChain = mdxgiFactory->CreateSwapChain(mCommandQueue, &sd, &mSwapChain);
	assert(SUCCEEDED(hrSwapChain));
}


void thisApp::FlushCommandQueue()
{
	mCurrentFence++;

	HRESULT hrSignal(mCommandQueue->Signal(mFence, mCurrentFence));
	assert(SUCCEEDED(hrSignal));

	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);
		HRESULT hrEventOnCompletion(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));
		assert(SUCCEEDED(hrEventOnCompletion));

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void thisApp::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	HRESULT hrDescrtv(md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));
	if (FAILED(hrDescrtv))
	{
		assert(FAILED(hrDescrtv) && "Failed to create RTV Descriptor Heap");
		std::cout << "Failed to create RTV Descriptor Heap" << std::endl;
	}
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	HRESULT hrDescdsv(md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap)));
	assert(SUCCEEDED(hrDescdsv));
}

void thisApp::CalculateFrame(HWND mainWin, wstring mMainWndCaption) {

	thisTime.Update();

	float fFps = thisTime.GetFPS();
	float mspf = 1000.0f / fFps;

	wstring fpsStr = to_wstring(fFps);
	wstring mspfStr = to_wstring(mspf);

	wstring windowText = mMainWndCaption +
		L"    fps: " + fpsStr +
		L"   mspf: " + mspfStr;

	SetWindowText(mainWin, windowText.c_str());
}



//Front and Back buffer

ID3D12Resource* thisApp::CurrentBackBuffer() {
	return tSwapChainBuffer[iCurrBackBuffer];
}

D3D12_CPU_DESCRIPTOR_HANDLE thisApp::CurrentBackBufferView() {
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), iCurrBackBuffer, mRtvDescriptorSize);
}

//Depth Buffer

D3D12_CPU_DESCRIPTOR_HANDLE thisApp::DepthStencilView() {
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

//Constant Buffer view (avec srv)
