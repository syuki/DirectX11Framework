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
_render_target_view(nullptr) {

  // �N���C�A���g�T�C�Y�v�Z
  _CalcClientSize(window_handle);

  // �X���b�v�`�F�C���ݒ�
  DXGI_SWAP_CHAIN_DESC swap_chain = { 0 };
  swap_chain.BufferDesc.Width = _client_width;
  swap_chain.BufferDesc.Height = _client_height;
  swap_chain.BufferDesc.RefreshRate.Denominator = 60;
  swap_chain.BufferDesc.RefreshRate.Numerator = 1;
  swap_chain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swap_chain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swap_chain.SampleDesc.Count = 1;
  swap_chain.SampleDesc.Quality = 0;
  swap_chain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain.BufferCount = 1;
  swap_chain.OutputWindow = window_handle;
  swap_chain.Windowed = true;
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

/// @fn ~SwapChain
/// @brief �f�X�g���N�^
SwapChain::~SwapChain() {
  SetOutput(nullptr, false);
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

  //desc.RefreshRate.Denominator = 0;
  //desc.RefreshRate.Numerator = 0;
  //_swap_chain->SetFullscreenState(is_full, output);
}

void SwapChain::ResizeBuffer(IDXGIOutput* output) {
  DXGI_MODE_DESC desc;
  _ASSERT_EXPR(output, L"SwapChain : output��null�ł�");
  UINT count;
  output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &count, nullptr);
  DXGI_MODE_DESC* desc_list = new DXGI_MODE_DESC[count];
  output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &count, desc_list);
  desc = desc_list[count - 1];
  delete[] desc_list;
  _swap_chain->ResizeTarget(&desc);
}

/// @fn Present
/// @brief �v���[���g
void SwapChain::Present() {
  _swap_chain->Present(0, 0);
}

//EOF