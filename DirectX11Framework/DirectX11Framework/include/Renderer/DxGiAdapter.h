/// @file DxGiAdapter.h
/// @brief DXGI�n
/// @date 2015/10/011
/// @author syuki nishida
#pragma once

#include "Renderer/SwapChain.h"
#include <Windows.h>
#include <dxgi.h>
#include "Util/SafeFunctions.h"
#include "Interface/ReferenceCountInterface.h"

#pragma comment(lib, "dxgi.lib")

/// @namespace snlib
namespace snlib {

/// @class DxGiAdapter
class DxGiAdapter: public ReferenceCountInterface {
public:
  /// @fn CreateDxGiAdapter
  /// @brief DxGi�쐬
  inline static DxGiAdapter* CreateDxGiAdapter() {
    return new DxGiAdapter();
  }

  /// @fn GetAdapter
  /// @brief �f�t�H���g�A�_�v�^�[�擾
  inline IDXGIAdapter1* GetAdapter() {
    return _default_adapter;
  }

  /// @fn CreateSwapChain
  /// @brief �X���b�v�`�F�C���쐬
  inline SwapChain* CreateSwapChain(ID3D11Device* device, HWND window_handle) {
    return SwapChain::CreateSwapChain(_factory, device, window_handle);
  }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn DxGiAdapter
  /// @brief �R���X�g���N�^
  inline DxGiAdapter() : ReferenceCountInterface("dxgi") {
    CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&_factory));
    _factory->EnumAdapters1(0, &_default_adapter);
  }

  /// @fn ~DxGiAdapter
  /// @brief �f�X�g���N�^
  inline virtual ~DxGiAdapter() {
    SafeRelease(_default_adapter);
    SafeRelease(_factory);
  }

  IDXGIFactory1* _factory;
  IDXGIAdapter1* _default_adapter;
};

/// @fn CreateDxGiAdapter
/// @brief DxGiAdapter�쐬
/// @return DxGiAdapter�̃A�h���X
inline DxGiAdapter* CreateDxGiAdapter() {
  return DxGiAdapter::CreateDxGiAdapter();
};

} // namespace snlib

//EOF