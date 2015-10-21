/// @file SwapChain.h
/// @brief �X���b�v�`�F�C��
/// @date 2015/10/08
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <Windows.h>
#include "Interface/ReferenceCountInterface.h"

/// @namespace
namespace snlib {

/// @class SwapChain
class SwapChain : public ReferenceCountInterface {
public:
  /// @fn CreateSwapChain
  /// @brief �X���b�v�`�F�C���쐬
  inline static SwapChain* CreateSwapChain(IDXGIFactory* factory, ID3D11Device* device, HWND window_handle) {
    return new SwapChain(factory, device, window_handle);
  }

  /// @fn SetOutput
  /// @brief �o�͐�ݒ�
  /// @param[in] output �ݒ肷��A�E�g�v�b�g
  /// @param[in] is_full �t���X�N���[����
  void SetOutput(IDXGIOutput* output, bool is_full);

  /// @fn ResizeBuffer
  /// @brief �o�b�t�@���T�C�Y
  /// @param[in] desc �o�b�t�@�ݒ�
  void ResizeBuffer(DXGI_MODE_DESC* desc, bool zero = false);

  /// @fn ResizeBuffer
  /// @brief �o�b�t�@���T�C�Y
  /// @param[in] output �A�E�g�v�b�g �Ō�̃T�C�Y�ɐݒ�
  void ResizeBuffer(IDXGIOutput* output, bool zero = false);

  /// @fn SetRenderTarget
  /// @brief �����_�[�^�[�Q�b�g�ɐݒ�
  inline void SetRenderTarget(ID3D11DeviceContext* context) {
    context->OMSetRenderTargets(1, &_render_target_view, _depth_stencil_view);
  }

  /// @fn ClearColor
  /// @brief �����_�[�^�[�Q�b�g�̐F�N���A
  inline void ClearColor(ID3D11DeviceContext* context) {
    float color[] = { 0.5f, 0, 0.5f, 1.f };
    context->ClearRenderTargetView(_render_target_view, color);
  }

  /// @fn CrearDepth
  /// @brief �[�x�N���A
  inline void ClearDepth(ID3D11DeviceContext* context) {
    context->ClearDepthStencilView(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
  }

  /// @fn Clear
  /// @brief �o�b�t�@�N���A
  inline void Clear(ID3D11DeviceContext* context) {
    ClearColor(context);
    ClearDepth(context);
  }

  /// @fn Present
  /// @brief �v���[���g
  void Present();

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

  /// @fn _InitDepthStencil
  /// @brief �[�x�o�b�t�@�쐬
  void _InitDepthStencil(ID3D11Device* device);

  /// @fn _UninitDepthStencil
  /// @brief �[�x�o�b�t�@�̏I��
  void _UninitDepthStencil();

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
  ID3D11Texture2D* _depth_buffer;
  ID3D11RenderTargetView* _render_target_view;
  ID3D11DepthStencilView* _depth_stencil_view;
};

} // namespace snlib

//EOF