/// @file SwapChain.cpp
/// @brief �X���b�v�`�F�C��
/// @date 2015/10/08
/// @author syuki nishida

#include "Renderer/SwapChain.h"
#include <crtdbg.h>

using namespace snlib;

/// @fn SwapChain
/// @brief �R���X�g���N�^
SwapChain::SwapChain(IDXGIFactory* factory, ID3D11Device* device, HWND window_handle) :
ReferenceCountInterface("swap_chain"),
_swap_chain(nullptr),
_back_buffer(nullptr),
_render_target_view(nullptr),
_depth_buffer(nullptr),
_depth_stencil_view(nullptr) {

  // �N���C�A���g�T�C�Y�v�Z
  _CalcClientSize(window_handle);

  // �X���b�v�`�F�C���ݒ�
  DXGI_SWAP_CHAIN_DESC swap_chain = { 0 };
  swap_chain.BufferDesc.Width = 1440;//_client_width;
  swap_chain.BufferDesc.Height = 900;//_client_height;
  swap_chain.BufferDesc.RefreshRate.Denominator = 1481600;
  swap_chain.BufferDesc.RefreshRate.Numerator = 88890000;
  swap_chain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swap_chain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swap_chain.SampleDesc.Count = 1;
  swap_chain.SampleDesc.Quality = 0;
  swap_chain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain.BufferCount = 1;
  swap_chain.OutputWindow = window_handle;
  swap_chain.Windowed = false;
  swap_chain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  swap_chain.Flags = 0;

  _window_mode_desc.Width = swap_chain.BufferDesc.Width;
  _window_mode_desc.Height = swap_chain.BufferDesc.Height;
  _window_mode_desc.RefreshRate = swap_chain.BufferDesc.RefreshRate;
  _window_mode_desc.Format = swap_chain.BufferDesc.Format;
  _window_mode_desc.ScanlineOrdering = swap_chain.BufferDesc.ScanlineOrdering;
  _window_mode_desc.Scaling = swap_chain.BufferDesc.Scaling;

  // �X���b�v�`�F�C���쐬
  HRESULT ret;
  ret = factory->CreateSwapChain(device, &swap_chain, &_swap_chain);
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : �X���b�v�`�F�C���쐬�Ɏ��s");

  _InitRenderTarget(device);
  _InitDepthStencil(device);
}

/// @fn _InitRenderTarget
/// @brief �����_�[�^�[�Q�b�g������
/// @param[in] device �f�o�C�X
void SwapChain::_InitRenderTarget(ID3D11Device* device) {
  HRESULT ret;

  _ASSERT_EXPR(!_back_buffer, L"SwapChain : �o�b�N�o�b�t�@�����ɂ���܂�");
  _ASSERT_EXPR(!_render_target_view, L"SwapChain : TargetView�����ɂ���܂�");

  // �o�b�N�o�b�t�@�擾
  ret = _swap_chain->GetBuffer(
    0,
    __uuidof(ID3D11Texture2D),
    reinterpret_cast<void**>(&_back_buffer));
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : �o�b�t�@�擾���s");

  // �����_�[�^�[�Q�b�g�r���[����
  ret = device->CreateRenderTargetView(
    _back_buffer,
    nullptr,
    &_render_target_view);
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : �����_�[�^�[�Q�b�g�쐬���s");
}

/// @fn _UninitRenderTarget
/// @brief �����_�[�^�[�Q�b�g�̏I��
void SwapChain::_UninitRenderTarget() {
  ULONG count;

  if (!_render_target_view) return;
  if (!_back_buffer) return;

  count = _render_target_view->Release();
  _ASSERT_EXPR(!count, L"SwapChain : �����_�[�^�[�Q�b�g�r���[�Ɋւ��ĊJ���~�X");

  count = _back_buffer->Release();
  _ASSERT_EXPR(!count, L"SwapChain : �o�b�N�o�b�t�@�Ɋւ��ĊJ���~�X");
}

/// @fn _InitDepthStencil
/// @brief �[�x�o�b�t�@�쐬
void SwapChain::_InitDepthStencil(ID3D11Device* device) {
  HRESULT ret;

  _ASSERT_EXPR(!_depth_buffer, L"SwapChain : �[�x�o�b�t�@�����ɂ���܂�");
  _ASSERT_EXPR(!_depth_stencil_view, L"SwapChain : DepthStencilView�����ɂ���܂�");

  DXGI_SWAP_CHAIN_DESC sc_desc;
  _swap_chain->GetDesc(&sc_desc);

  D3D11_TEXTURE2D_DESC tex_desc;
  tex_desc.Width = sc_desc.BufferDesc.Width;
  tex_desc.Height = sc_desc.BufferDesc.Height;
  tex_desc.MipLevels = 1;
  tex_desc.ArraySize = 1;
  tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
  tex_desc.SampleDesc.Count = sc_desc.SampleDesc.Count; 
  tex_desc.SampleDesc.Quality = sc_desc.SampleDesc.Quality;
  tex_desc.Usage = D3D11_USAGE_DEFAULT;
  tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  tex_desc.CPUAccessFlags = 0;
  tex_desc.MiscFlags = 0;

  ret = device->CreateTexture2D(&tex_desc, nullptr, &_depth_buffer);
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : �[�x�e�N�X�`���쐬���s");

  D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
  dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsv_desc.Flags = 0;

  ret = device->CreateDepthStencilView(_depth_buffer, &dsv_desc, &_depth_stencil_view);
  _ASSERT_EXPR(SUCCEEDED(ret), L"SwapChain : �[�x�r���[�̍쐬�Ɏ��s");
}

/// @fn _UninitDepthStencil
/// @brief �[�x�o�b�t�@�̏I��
void SwapChain::_UninitDepthStencil() {
  ULONG count;

  if (!_depth_buffer) return;
  if (!_depth_stencil_view) return;

  count = _depth_stencil_view->Release();
  _ASSERT_EXPR(!count, L"SwapChain : �[�x�r���[�̊J���~�X");

  count = _depth_buffer->Release();
  _ASSERT_EXPR(!count, L"SwapChain : �[�x�o�b�t�@�̊J���~�X");
}

/// @fn ~SwapChain
/// @brief �f�X�g���N�^
SwapChain::~SwapChain() {
  SetOutput(nullptr, false);
  _UninitDepthStencil();
  _UninitRenderTarget();
  ULONG count = _swap_chain->Release();
  _ASSERT_EXPR(!count, L"SwapChain : �J���~�X������܂�");
}

/// @fn SetOutput
/// @brief �o�͐�ݒ�
/// @param[in] output �ݒ肷��A�E�g�v�b�g
/// @param[in] is_full �t���X�N���[����
void SwapChain::SetOutput(IDXGIOutput* output, bool is_full) {
  _swap_chain->SetFullscreenState(is_full, output);
}

/// @fn ResizeBuffer
/// @brief �o�b�t�@���T�C�Y
/// @param[in] desc �o�b�t�@�ݒ�
void SwapChain::ResizeBuffer(DXGI_MODE_DESC* desc, bool zero) {
  DXGI_MODE_DESC mode_desc;
  if (desc){
    mode_desc = *desc;
  }
  else {
    mode_desc = _window_mode_desc;
  }
  if (zero) {
    //mode_desc.RefreshRate.Denominator = 0;
    //mode_desc.RefreshRate.Numerator = 0;
  }
  _swap_chain->ResizeTarget(&mode_desc);
}

/// @fn ResizeBuffer
/// @brief �o�b�t�@���T�C�Y
/// @param[in] output �A�E�g�v�b�g �Ō�̃T�C�Y�ɐݒ�
void SwapChain::ResizeBuffer(IDXGIOutput* output, bool zero) {
  _ASSERT_EXPR(output, L"SwapChain : output��null�ł�");
  UINT count;
  output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &count, nullptr);
  DXGI_MODE_DESC* desc_list = new DXGI_MODE_DESC[count];
  output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &count, desc_list);
  ResizeBuffer(&desc_list[count - 1], zero);
  delete[] desc_list;
}


/// @fn Present
/// @brief �v���[���g
void SwapChain::Present() {
  _swap_chain->Present(1, 0);
}

//EOF