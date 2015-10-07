/// @file DirectX11Module.h
/// @brief DirectX11モジュール
/// @date 2015/10/02
/// @author syuki nishida
#pragma once

#include <Windows.h>
#include <D3D11.h>
#include "Interface/ReferenceCountInterface.h"

/// @namespace snlib
namespace snlib {

/// @class DirectX11Module
class DirectX11Module : public ReferenceCountInterface {
public:
  /// @fn createDirectX11Module
  /// @brief DirectX11Module作成
  /// @param[in] window_handle ウィンドウハンドル
  static DirectX11Module* CreateDirectX11Module(HWND window_handle);

  /// @fn GetWindowHandle
  /// @brief ウィンドウハンドル取得
  /// @return ウィンドウハンドル
  inline const HWND GetWindowHandle() { return _window_handle; }

  void render();

protected:
private:
  /// @fn DirectX11Module
  /// @brief コンストラクタ
  /// @param[in] window_handle ウィンドウハンドル
  DirectX11Module(HWND window_handle);

  /// @fn ~DirectX11Module
  /// @brief デストラクタ
  virtual ~DirectX11Module();

  /// @fn _CalcClientSize
  /// @brief クライアント領域のサイズ計算
  inline void _CalcClientSize() {
    RECT client_size;
    GetClientRect(_window_handle, &client_size);
    _client_width = client_size.right - client_size.left;
    _client_height = client_size.bottom - client_size.top;
  }

  HWND _window_handle;
  LONG _client_width;
  LONG _client_height;
  ID3D11Device* _device;
  D3D_FEATURE_LEVEL _feature_level;
  ID3D11DeviceContext* _device_context;
  D3D_DRIVER_TYPE _driver_type;
  IDXGISwapChain* _swap_chain;
  ID3D11RenderTargetView* _render_target_view;

};

} // namespace snlib

//EOF