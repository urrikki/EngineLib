#include "pch.h"
#include "Initdx.h"


using namespace std;

thisApp::thisApp() {}

thisApp::~thisApp() {}

void thisApp::Initialize() {
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

mTime.Start();

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
	//&mSwapChain->Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
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

	HRESULT hrSwapChain(mdxgiFactory->CreateSwapChain(mCommandQueue, &sd, &mSwapChain));
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
	assert(SUCCEEDED(hrDescrtv));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	HRESULT hrDescdsv(md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap)));
	assert(SUCCEEDED(hrDescdsv));
}

float fTimeElapsed = 0.0f;

void thisApp::CalculateFrame(HWND mainWin, wstring mMainWndCaption, Time& thisTime) {

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

//Depth Buffer

//Constant Buffer view (avec srv)
