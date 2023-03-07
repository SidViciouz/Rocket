#pragma once

#include "Util.h"
#include "IfError.h"
#include "Frame.h"

using namespace Microsoft::WRL;
using namespace std;

class Pipeline
{
public:
	static ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12Fence> mFence = nullptr;
	UINT64 mFenceValue = 0;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D12Resource> mBackBuffers[2];
	int mCurrentBackBuffer = 0;
	unordered_map<string, ComPtr< ID3D12PipelineState>> mPSOs;
	D3D12_VIEWPORT mViewport;
	D3D12_RECT mScissor;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	unordered_map<string,ComPtr<ID3D12RootSignature>> mRootSignatures;

	Pipeline(const int& width,const int& height);
	void Initialize();
	void CreateObjects(HWND windowHandle,int numModels);

private:
	void CreateBackBuffersAndDepthBufferAndViews();
	void CreateDescriptorHeaps();
	void CreateSwapChain(HWND windowHandle);
	void CreateShaderAndRootSignature();
	void CreatePso();
	void SetViewportAndScissor();

	const int& mWidth;
	const int& mHeight;

	ComPtr<IDXGIFactory4> mFactory = nullptr;

	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	bool mMsaaEnable = false;
	UINT mMsaaQuality = 0;

	ComPtr<ID3D12Resource> mDepthBuffer;

	unordered_map<string, ComPtr<ID3DBlob>> mShaders;

	/*
	ComPtr<ID3D12DescriptorHeap> mSrvHeap = nullptr;
	ComPtr<ID3D12DescriptorHeap> mVolumeUavHeap = nullptr;
	ComPtr<ID3D12DescriptorHeap> mVolumeUavHeapInvisible = nullptr;
	*/
};
