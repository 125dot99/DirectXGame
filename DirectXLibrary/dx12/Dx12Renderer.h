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
	static ComPtr<ID3D12Device> com_pDevice;
	//描画コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> com_pCommandList;
	//深度バッファデスクリプタヒープ
	static std::unique_ptr<class DescriptorHeap> u_pDescriptorHeapDSV;
	//ウィンドウサイズをキャッシュ
	Vector2 screenSize;
	//DXGIファクトリー
	ComPtr<IDXGIFactory6> com_pDxgiFactory;
	//画面設定
	ComPtr<IDXGISwapChain4> com_pSwapchain;
	//コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> com_pCommandAllocator;
	//コマンドリストの送信、同期
	ComPtr<ID3D12CommandQueue> com_pCommandQueue;
	//バッファのリソース(表：裏)
	static const char FRAME_BUFFER_SIZE = 2;
	ComPtr<ID3D12Resource> com_pBackBuffers[FRAME_BUFFER_SIZE];
	//レンダーターゲット
	std::unique_ptr<class DescriptorHeap> u_pDescriptorHeapRTV;
	//深度バッファ
	ComPtr<ID3D12Resource> com_pDepthResource;
	//コマンドリストの終了検知
	ComPtr<ID3D12Fence> com_pFence;
	UINT64 fenceValue;
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
		return com_pDevice.Get();
	}
	//読み取り可能コマンドリスト
	inline static ID3D12GraphicsCommandList* GetCommandList()
	{
		return com_pCommandList.Get();
	}
	//深度バッファデスクリプタヒープ
	inline static DescriptorHeap* GetDescriptorHespDSV()
	{
		return u_pDescriptorHeapDSV.get();
	}
};
} // namespace gamelib