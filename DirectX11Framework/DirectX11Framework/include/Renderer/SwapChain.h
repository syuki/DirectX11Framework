/// @file SwapChain.h
/// @brief スワップチェイン
/// @date 2015/10/08
/// @author syuki nishida
#pragma once

#include <D3D11.h>
#include "Interface/ReferenceCountInterface.h"

/// @namespace
namespace snlib {

/// @class SwapChain
class SwapChain : public ReferenceCountInterface {
  friend class DirectX11Module;
public:
  /// @fn SetOutput
  /// @brief 出力先設定
  /// @param[in] output 設定するアウトプット
  /// @param[in] is_full フルスクリーンか
  void SetOutput(IDXGIOutput* output, bool is_full);

  void ResizeBuffer(IDXGIOutput* output);

  /// @fn Present
  /// @brief プレゼント
  void Present();

  /// @fn GetRenderTargetView
  /// @brief レンダーターゲットビュー取得
  ID3D11RenderTargetView* GetRenderTargetView() { return _render_target_view; }

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
  ID3D11RenderTargetView* _render_target_view;
};

} // namespace snlib

//EOF