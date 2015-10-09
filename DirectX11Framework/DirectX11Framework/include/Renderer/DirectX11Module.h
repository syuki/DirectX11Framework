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

class SwapChain;

/// @class DirectX11Module
class DirectX11Module : public ReferenceCountInterface {
public:
  /// @fn CreateDirectX11Module
  /// @brief DirectX11Module作成
  static DirectX11Module* CreateDirectX11Module();

  /// @fn CreateSwapChain
  /// @brief スワップチェイン作成
  SwapChain* CreateSwapChain(HWND window_handle);

  /// @fn Start
  /// @brief 始める
  void Start();

  /// @fn GetDevice
  /// @brief デバイス取得
  ID3D11Device* GetDevice() { return _device; }

  /// @fn GetDeviceContext
  /// @brief デバイスコンテキストの取得
  ID3D11DeviceContext* GetDeviceContext() { return _device_context; }

  /// @fn GetDXGIFactory
  /// @brief DXGIFactory取得
  IDXGIFactory1* GetDXGIFactory() { return _factory; }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn DirectX11Module
  /// @brief コンストラクタ
  DirectX11Module();

  /// @fn ~DirectX11Module
  /// @brief デストラクタ
  virtual ~DirectX11Module();

  /// @fn _CreateDevice
  /// @brief デバイス作成
  /// @param[in] adapter アダプタ
  void _CreateDevice(IDXGIAdapter1* adapter = nullptr);

  /// @fn DeleteDevice
  /// @brief デバイス削除
  void _DeleteDevice();

  IDXGIFactory1*        _factory;
  ID3D11Device*         _device;
  ID3D11DeviceContext*  _device_context;
  D3D_FEATURE_LEVEL     _feature_level;
  D3D_DRIVER_TYPE   _driver_type;
};

} // namespace snlib

//EOF