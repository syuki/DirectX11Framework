/// @file DirectX11Module.h
/// @brief DirectX11���W���[��
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
  /// @brief DirectX11Module�쐬
  static DirectX11Module* CreateDirectX11Module();

  /// @fn CreateSwapChain
  /// @brief �X���b�v�`�F�C���쐬
  SwapChain* CreateSwapChain(HWND window_handle);

  /// @fn Start
  /// @brief �n�߂�
  void Start();

  /// @fn GetDevice
  /// @brief �f�o�C�X�擾
  ID3D11Device* GetDevice() { return _device; }

  /// @fn GetDeviceContext
  /// @brief �f�o�C�X�R���e�L�X�g�̎擾
  ID3D11DeviceContext* GetDeviceContext() { return _device_context; }

  /// @fn GetDXGIFactory
  /// @brief DXGIFactory�擾
  IDXGIFactory1* GetDXGIFactory() { return _factory; }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn DirectX11Module
  /// @brief �R���X�g���N�^
  DirectX11Module();

  /// @fn ~DirectX11Module
  /// @brief �f�X�g���N�^
  virtual ~DirectX11Module();

  /// @fn _CreateDevice
  /// @brief �f�o�C�X�쐬
  /// @param[in] adapter �A�_�v�^
  void _CreateDevice(IDXGIAdapter1* adapter = nullptr);

  /// @fn DeleteDevice
  /// @brief �f�o�C�X�폜
  void _DeleteDevice();

  IDXGIFactory1*        _factory;
  ID3D11Device*         _device;
  ID3D11DeviceContext*  _device_context;
  D3D_FEATURE_LEVEL     _feature_level;
  D3D_DRIVER_TYPE   _driver_type;
};

} // namespace snlib

//EOF