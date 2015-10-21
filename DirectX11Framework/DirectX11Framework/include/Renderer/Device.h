/// @file Device.h
/// @brief デバイス
/// @date 2015/10/11
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include "Interface/ReferenceCountInterface.h"
#include "Renderer/Context.h"

#pragma comment(lib, "d3d11.lib")

/// @namespace snlib
namespace snlib {

/// @class Device
class Device : public ReferenceCountInterface {
public:
  /// @fn CreateDevice
  /// @brief デバイス作成
  inline static Device* CreateDevice() {
    return new Device();
  }

  /// @fn GetContext
  /// @brief コンテキスト取得
  inline Context* GetContext() {
    return Context::CreateContext(_context);
  }

  /// @fn GetDevice
  /// @brief Dx11のデバイス取得
  /// @retun デバイスのアドレス
  inline ID3D11Device* GetDevice() {
    return _device;
  }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn Device
  /// @brief コンストラクタ
  Device();

  /// @fn ~Device
  /// @brief デストラクタ
  virtual ~Device();

  ID3D11Device*        _device;
  ID3D11DeviceContext* _context;

  D3D_FEATURE_LEVEL    _feature_level;
  D3D_DRIVER_TYPE      _driver_type;
};

/// @fn CreateDevice
/// @brief デバイス作成
/// @return デバイスアドレス
inline Device* CreateDevice() {
  return Device::CreateDevice();
}

} // namespace snlib

//EOF