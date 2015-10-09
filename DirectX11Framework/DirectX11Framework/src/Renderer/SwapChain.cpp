/// @file SwapChain.cpp
/// @brief スワップチェイン
/// @date 2015/10/08
/// @author syuki nishida

#include "Renderer/SwapChain.h"
#include <crtdbg.h>

using namespace snlib;

/// @fn SwapChain
/// @brief コンストラクタ
SwapChain::SwapChain(IDXGIFactory* factory, ID3D11Device* device, HWND window_handle) :
ReferenceCountInterface("swap_chain"),
_swap_chain(nullptr),
_back_buffer(nullptr),
_render_target_view(nullptr) {

  // クライアントサイズ計算
  _CalcClientSize(window_handle);

  // スワップチェイン設定
  DXGI_SWAP_CHAIN_DESC swap_chain = { 0 };
  swap_chain.BufferDesc.Width = _client_width;
  swap_chain.BufferDesc.Height = _client_height;
  swap_chain.BufferDesc.RefreshRate.Denominator = 60;
  swap_chain.BufferDesc.RefreshRate.Numerator = 1;
  swap_chain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swap_chain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swap_chain.SampleDesc.Count = 1;
  swap_chain.SampleDesc.Quality = 0;
  swap_chain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain.BufferCount = 1;
  swap_chain.OutputWindow = window_handle;
  swap_chain.Windowed = true;
  swap_chain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  swap_chain.Flags = 0;

  _window_mode_desc.Width = swap_chain.BufferDesc.Width;
  _window_mode_desc.Height = swap_chain.BufferDesc.Height;
  _window_mode_desc.RefreshRate = swap_chain.BufferDesc.RefreshRate;
  _window_mode_desc.Format = swap_chain.BufferDesc.Format;
  _window_mode_desc.ScanlineOrdering = swap_chain.BufferDesc.ScanlineOrdering;
  _window_mode_desc.Scaling = swap_chain.BufferDesc.Scaling;

  // スワップチェイン作成
  HRESULT ret;
  ret = factory->CreateSwapChain(device, &swap_chain, &_swap_chain);
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : スワップチェイン作成に失敗");

  _InitRenderTarget(device);
}

/// @fn _InitRenderTarget
/// @brief レンダーターゲット初期化
/// @param[in] device デバイス
void SwapChain::_InitRenderTarget(ID3D11Device* device) {
  HRESULT ret;

  _ASSERT_EXPR(!_back_buffer, L"SwapChain : バックバッファが既にあります");
  _ASSERT_EXPR(!_render_target_view, L"SwapChain : TargetViewが既にあります");

  // バックバッファ取得
  ret = _swap_chain->GetBuffer(
    0,
    __uuidof(ID3D11Texture2D),
    reinterpret_cast<void**>(&_back_buffer));
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : バッファ取得失敗");

  // レンダーターゲットビュー生成
  ret = device->CreateRenderTargetView(
    _back_buffer,
    nullptr,
    &_render_target_view);
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : レンダーターゲット作成失敗");
}

/// @fn _UninitRenderTarget
/// @brief レンダーターゲットの終了
void SwapChain::_UninitRenderTarget() {
  ULONG count;

  if (!_render_target_view) return;
  if (!_back_buffer) return;

  count = _render_target_view->Release();
  _ASSERT_EXPR(!count, L"SwapChain : レンダーターゲットビューに関して開放ミス");

  count = _back_buffer->Release();
  _ASSERT_EXPR(!count, L"SwapChain : バックバッファに関して開放ミス");
}

/// @fn ~SwapChain
/// @brief デストラクタ
SwapChain::~SwapChain() {
  SetOutput(nullptr, false);
  _UninitRenderTarget();
  ULONG count = _swap_chain->Release();
  _ASSERT_EXPR(!count, L"SwapChain : 開放ミスがあります");
}

/// @fn SetOutput
/// @brief 出力先設定
/// @param[in] output 設定するアウトプット
/// @param[in] is_full フルスクリーンか
void SwapChain::SetOutput(IDXGIOutput* output, bool is_full) {


  _swap_chain->SetFullscreenState(is_full, output);

  //desc.RefreshRate.Denominator = 0;
  //desc.RefreshRate.Numerator = 0;
  //_swap_chain->SetFullscreenState(is_full, output);
}

void SwapChain::ResizeBuffer(IDXGIOutput* output) {
  DXGI_MODE_DESC desc;
  _ASSERT_EXPR(output, L"SwapChain : outputがnullです");
  UINT count;
  output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &count, nullptr);
  DXGI_MODE_DESC* desc_list = new DXGI_MODE_DESC[count];
  output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &count, desc_list);
  desc = desc_list[count - 1];
  delete[] desc_list;
  _swap_chain->ResizeTarget(&desc);
}

/// @fn Present
/// @brief プレゼント
void SwapChain::Present() {
  _swap_chain->Present(0, 0);
}

//EOF