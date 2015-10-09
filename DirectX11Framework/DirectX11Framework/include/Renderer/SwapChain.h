/// @file SwapChain.h
/// @brief �X���b�v�`�F�C��
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
  /// @brief �o�͐�ݒ�
  /// @param[in] output �ݒ肷��A�E�g�v�b�g
  /// @param[in] is_full �t���X�N���[����
  void SetOutput(IDXGIOutput* output, bool is_full);

  void ResizeBuffer(IDXGIOutput* output);

  /// @fn Present
  /// @brief �v���[���g
  void Present();

  /// @fn GetRenderTargetView
  /// @brief �����_�[�^�[�Q�b�g�r���[�擾
  ID3D11RenderTargetView* GetRenderTargetView() { return _render_target_view; }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn SwapChain
  /// @brief �R���X�g���N�^
  SwapChain(IDXGIFactory* factory, ID3D11Device* device, HWND window_handle);

  /// @fn ~SwapChain
  /// @brief �f�X�g���N�^
  virtual ~SwapChain();

  /// @fn _InitRenderTarget
  /// @brief �����_�[�^�[�Q�b�g������
  /// @param[in] device �f�o�C�X
  void _InitRenderTarget(ID3D11Device* device);

  /// @fn _UninitRenderTarget
  /// @brief �����_�[�^�[�Q�b�g�̏I��
  void _UninitRenderTarget();

  /// @fn _CalcClientSize
  /// @brief �N���C�A���g�̈�̃T�C�Y�v�Z
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