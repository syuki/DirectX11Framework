/// @file Device.h
/// @brief �f�o�C�X
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
  /// @brief �f�o�C�X�쐬
  inline static Device* CreateDevice() {
    return new Device();
  }

  /// @fn GetContext
  /// @brief �R���e�L�X�g�擾
  inline Context* GetContext() {
    return Context::CreateContext(_context);
  }

  /// @fn GetDevice
  /// @brief Dx11�̃f�o�C�X�擾
  /// @retun �f�o�C�X�̃A�h���X
  inline ID3D11Device* GetDevice() {
    return _device;
  }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn Device
  /// @brief �R���X�g���N�^
  Device();

  /// @fn ~Device
  /// @brief �f�X�g���N�^
  virtual ~Device();

  ID3D11Device*        _device;
  ID3D11DeviceContext* _context;

  D3D_FEATURE_LEVEL    _feature_level;
  D3D_DRIVER_TYPE      _driver_type;
};

/// @fn CreateDevice
/// @brief �f�o�C�X�쐬
/// @return �f�o�C�X�A�h���X
inline Device* CreateDevice() {
  return Device::CreateDevice();
}

} // namespace snlib

//EOF