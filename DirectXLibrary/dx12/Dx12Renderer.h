#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>

#include "../math/Vector.h"

namespace gamelib
{
//DirectXの基盤をまとめたクラス
class Dx12Renderer
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//生成用デバイス
	static ComPtr<ID3D12Device> device;
	//描画コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> commandList;
	//デプスステンシルビューのハンドル
	static D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	//ウィンドウサイズをキャッシュ
	Vector2 screenSize;
	//DXGIファクトリー
	ComPtr<IDXGIFactory6> dxgiFactory;
	//画面設定
	ComPtr<IDXGISwapChain4> swapchain;
	//コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//コマンドリストの送信、同期
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//バッファのリソース(表：裏)
	static const char FRAME_BUFFER_SIZE = 2;
	ComPtr<ID3D12Resource> backBuffers[FRAME_BUFFER_SIZE];
	//レンダーターゲット
	std::unique_ptr<class DescriptorHeap> descriptorHeapRTV;
	//コマンドリストの終了検知
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValue;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;
	//深度値デスクリプタヒープ
	std::unique_ptr<class DescriptorHeap> descriptorHeapDSV;
	//メモリリーク検知用
	void OnDebugLayer();
	void CreateDevice();
	void CreateCommand();
	void CreateSwapChain();
	void CreateRenderTarget();
	void CreateDepthBuffer();
public:
	//DirectX12デバイスの生成
	void Initialize();
	//画面に表示されているものを消す
	void DrawBegin();
	//画面を表示状態にする
	void DrawEnd();
	//読み取り可能デバイス
	inline static ID3D12Device* GetDevice()
	{
		return device.Get();
	}
	//読み取り可能コマンドリスト
	inline static ID3D12GraphicsCommandList* GetCommandList()
	{
		return commandList.Get();
	}
	inline static const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSVDescriptorCPUHandle()
	{
		return dsvHandle;
	}
};
} // namespace gamelib