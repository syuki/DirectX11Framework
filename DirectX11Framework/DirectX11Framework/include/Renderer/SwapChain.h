/// @file SwapChain.h
/// @brief スワップチェイン
/// @date 2015/10/08
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <Windows.h>
#include "Interface/ReferenceCountInterface.h"

/// @namespace
namespace snlib {

/// @class SwapChain
class SwapChain : public ReferenceCountInterface {
public:
  /// @fn CreateSwapChain
  /// @brief スワップチェイン作成
  inline static SwapChain* CreateSwapChain(IDXGIFactory* factory, ID3D11Device* device, HWND window_handle) {
    return new SwapChain(factory, device, window_handle);
  }

  /// @fn SetOutput
  /// @brief 出力先設定
  /// @param[in] output 設定するアウトプット
  /// @param[in] is_full フルスクリーンか
  void SetOutput(IDXGIOutput* output, bool is_full);

  /// @fn ResizeBuffer
  /// @brief バッファリサイズ
  /// @param[in] desc バッファ設定
  void ResizeBuffer(DXGI_MODE_DESC* desc, bool zero = false);

  /// @fn ResizeBuffer
  /// @brief バッファリサイズ
  /// @param[in] output アウトプット 最後のサイズに設定
  void ResizeBuffer(IDXGIOutput* output, bool zero = false);

  /// @fn SetRenderTarget
  /// @brief レンダーターゲットに設定
  inline void SetRenderTarget(ID3D11DeviceContext* context) {
    context->OMSetRenderTargets(1, &_render_target_view, _depth_stencil_view);
  }

  /// @fn ClearColor
  /// @brief レンダーターゲットの色クリア
  inline void ClearColor(ID3D11DeviceContext* context) {
    float color[] = { 0.5f, 0, 0.5f, 1.f };
    context->ClearRenderTargetView(_render_target_view, color);
  }

  /// @fn CrearDepth
  /// @brief 深度クリア
  inline void ClearDepth(ID3D11DeviceContext* context) {
    context->ClearDepthStencilView(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
  }

  /// @fn Clear
  /// @brief バッファクリア
  inline void Clear(ID3D11DeviceContext* context) {
    ClearColor(context);
    ClearDepth(context);
  }

  /// @fn Present
  /// @brief プレゼント
  void Present();

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn SwapChain
  /// @brief コンストラクタ
  SwapChain(IDXGIFactory* factory, ID3D11Device* device, HWND window_handle);

  /// @fn ~SwapChain
  /// @brief デストラクタ
  virtual ~SwapChain();

  /// @fn _InitRenderTarget
  /// @brief レンダーターゲット初期化
  /// @param[in] device デバイス
  void _InitRenderTarget(ID3D11Device* device);

  /// @fn _UninitRenderTarget
  /// @brief レンダーターゲットの終了
  void _UninitRenderTarget();

  /// @fn _InitDepthStencil
  /// @brief 深度バッファ作成
  void _InitDepthStencil(ID3D11Device* device);

  /// @fn _UninitDepthStencil
  /// @brief 深度バッファの終了
  void _UninitDepthStencil();

  /// @fn _CalcClientSize
  /// @brief クライアント領域のサイズ計算
  inline void _CalcClientSize(HWND window_handle) {
    RECT client_size;
    GetClientRect(window_handle, &client_size);
    _client_width = client_size.right - client_size.left;
    _client_height = client_size.bottom - client_size.top;
  }

  IDXGISwapChain* _swap_chain;
  DXGI_MODE_DESC _window_mode_desc;
  long _client_width;
  long _client_height;

  ID3D11Texture2D* _back_buffer;
  ID3D11Texture2D* _depth_buffer;
  ID3D11RenderTargetView* _render_target_view;
  ID3D11DepthStencilView* _depth_stencil_view;
};

} // namespace snlib

//EOF