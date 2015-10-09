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
  /// @fn createDirectX11Module
  /// @brief DirectX11Module�쐬
  /// @param[in] window_handle �E�B���h�E�n���h��
  static DirectX11Module* CreateDirectX11Module(HWND window_handle);

  /// @fn CreateSwapChain
  /// @brief �X���b�v�`�F�C���쐬
  SwapChain* CreateSwapChain(HWND window_handle);

  /// @fn Start
  /// @brief �n�߂�
  void Start();

  /// @fn GetWindowHandle
  /// @brief �E�B���h�E�n���h���擾
  /// @return �E�B���h�E�n���h��
  inline const HWND GetWindowHandle() { return _window_handle; }

  void render();

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn DirectX11Module
  /// @brief �R���X�g���N�^
  /// @param[in] window_handle �E�B���h�E�n���h��
  DirectX11Module(HWND window_handle);

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

  /// @fn _CalcClientSize
  /// @brief �N���C�A���g�̈�̃T�C�Y�v�Z
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
  IDXGIFactory1* _factory;
  D3D_FEATURE_LEVEL _feature_level;
  ID3D11DeviceContext* _device_context;
  ID3D11Texture2D* _back_buffer;
  D3D_DRIVER_TYPE _driver_type;

  SwapChain* _swap_chain;

};

} // namespace snlib

//EOF