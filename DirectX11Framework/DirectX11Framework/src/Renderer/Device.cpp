/// @file Device.cpp
/// @brief �f�o�C�X
/// @date 2015/10/11
/// @author syuki nishida

#include "Renderer/Device.h"
#include <crtdbg.h>
#include "Util/SafeFunctions.h"

using namespace snlib;

/// @fn Device
/// @brief �R���X�g���N�^
Device::Device() : ReferenceCountInterface("dx11device") {
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
      nullptr,
      driver_type[i],
      nullptr,
      device_frag,
      feature_level,
      feature_level_count,
      D3D11_SDK_VERSION,
      &_device,
      &_feature_level,
      &_context);
    if (SUCCEEDED(ret)) {
      _driver_type = driver_type[i];
      break;
    }
  }
  _ASSERT_EXPR(SUCCEEDED(ret), L"Device : �f�o�C�X�̍쐬�Ɏ��s");
}

/// @fn ~Device
/// @brief �f�X�g���N�^
Device::~Device() {
  SafeRelease(_context);
  SafeRelease(_device);
}

//EOF