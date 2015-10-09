/// @file DirectX11Module.cpp
/// @brief DirectX11���W���[��
/// @date 2015/10/02
/// @author syuki nishida

#include "Renderer/DirectX11Module.h"
#include "Renderer/SwapChain.h"
#include "Util/SafeFunctions.h"
#include <vector>
#include <crtdbg.h>
#include <errno.h>
#include <Util/RawData.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment(lib, "dxgi.lib")

using namespace snlib;

/// @fn createDirectX11Module
/// @brief DirectX11Module�쐬
DirectX11Module* DirectX11Module::CreateDirectX11Module() {
  return new DirectX11Module();
}

/// @fn CreateSwapChain
/// @brief �X���b�v�`�F�C���쐬
SwapChain* DirectX11Module::CreateSwapChain(HWND window_handle) {
  return new SwapChain(_factory, _device, window_handle);
}

/// @fn DirectX11Module
/// @brief �R���X�g���N�^
DirectX11Module::DirectX11Module() :
ReferenceCountInterface("dx11module"),
_factory(nullptr),
_device(nullptr),
_device_context(nullptr) {

  // �t�@�N�g���[�쐬
  CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&_factory));

  _CreateDevice();
}

/// @fn ~DirectX11Module
/// @brief �f�X�g���N�^
DirectX11Module::~DirectX11Module() {
  _DeleteDevice();
  SafeRelease(_factory);
}

/// @fn _CreateDevice
/// @brief �f�o�C�X�쐬
/// @param[in] adapter �A�_�v�^
void DirectX11Module::_CreateDevice(IDXGIAdapter1* adapter) {

  _ASSERT_EXPR(!_device, L"DirectX11Framework : �f�o�C�X�����ɂ���܂�");
  _ASSERT_EXPR(!_device_context, L"DirectX11Framework : Context�����ɂ���܂�");

  // �h���C�o�^�C�v�ݒ�
  D3D_DRIVER_TYPE driver_type[] = {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  ULONG driver_type_count = sizeof(driver_type) / sizeof(D3D_DRIVER_TYPE);

  // �@�\���x���ݒ�
  D3D_FEATURE_LEVEL feature_level[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  ULONG feature_level_count = sizeof(feature_level) / sizeof(D3D_FEATURE_LEVEL);

  // �f�o�C�X�t���O�ݒ�
  UINT device_frag = 0;
#ifdef _DEBUG
  device_frag = D3D11_CREATE_DEVICE_DEBUG;
#endif

  HRESULT ret;
  for (ULONG i = 0; i < driver_type_count; i++) {
    ret = D3D11CreateDevice(
      adapter,
      driver_type[i],
      nullptr,
      device_frag,
      feature_level,
      feature_level_count,
      D3D11_SDK_VERSION,
      &_device,
      &_feature_level,
      &_device_context);
    if (SUCCEEDED(ret)) {
      _driver_type = driver_type[i];
      break;
    }
  }
  _ASSERT_EXPR(SUCCEEDED(ret), L"DirectX11Framework : �f�o�C�X�̍쐬�Ɏ��s");
}

/// @fn DeleteDevice
/// @brief �f�o�C�X�폜
void DirectX11Module::_DeleteDevice() {
  SafeRelease(_device_context);
  SafeRelease(_device);
}

//EOF