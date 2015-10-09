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
/// @param[in] window_handle �E�B���h�E�n���h��
DirectX11Module* DirectX11Module::CreateDirectX11Module(HWND window_handle) {
  return new DirectX11Module(window_handle);
}

/// @fn CreateSwapChain
/// @brief �X���b�v�`�F�C���쐬
SwapChain* DirectX11Module::CreateSwapChain(HWND window_handle) {
  return new SwapChain(_factory, _device, window_handle);
}

ID3D11VertexShader* vertex_shader;
ID3D11PixelShader* pixel_shader;
ID3D11GeometryShader* geometry_shader;
ID3D11InputLayout* layout;
ID3D11Buffer* vertex_buffer;

bool LoadToCreateVertexShader(ID3D11Device* device) {
  // ���_�V�F�[�_�ǂݍ���
  RawData vertex_shader_data("data/shader/vertex.cso");
  if (vertex_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"vs load error");
    return false;
  }

  // ���_�V�F�[�_�쐬
  HRESULT ret = device->CreateVertexShader(vertex_shader_data.GetPointer(),
    vertex_shader_data.GetSize(),
    nullptr,
    &vertex_shader);
  vertex_shader_data.Free();

  return SUCCEEDED(ret);
}

bool LoadToCreatePixelShader(ID3D11Device* device) {
  // �s�N�Z���V�F�[�_�ǂݍ���
  RawData pixel_shader_data("data/shader/pixel.cso");
  if (pixel_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"ps load error");
    return false;
  }

  // �s�N�Z���V�F�[�_�쐬
  HRESULT ret = device->CreatePixelShader(pixel_shader_data.GetPointer(),
    pixel_shader_data.GetSize(),
    nullptr,
    &pixel_shader);
  pixel_shader_data.Free();

  return SUCCEEDED(ret);
}

bool LoadToCreateGeometryShader(ID3D11Device* device) {
  // �W�I���g���V�F�[�_�ǂݍ���
  RawData geometry_shader_data("data/shader/geometry.cso");
  if (geometry_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"gs load error");
    return false;
  }

  // �W�I���g���V�F�[�_�쐬
  HRESULT ret = device->CreateGeometryShader(geometry_shader_data.GetPointer(),
    geometry_shader_data.GetSize(),
    nullptr,
    &geometry_shader);
  geometry_shader_data.Free();

  return SUCCEEDED(ret);
}

bool CreateInputLayout(ID3D11Device* device, ID3D11DeviceContext* device_context) {
  // ���_�V�F�[�_�ǂݍ���
  RawData vertex_shader_data("data/shader/vertex.cso");
  if (vertex_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"vs load error");
    return false;
  }

  // ���̓��C�A�E�g��`
  D3D11_INPUT_ELEMENT_DESC elem_layout[] = {
      {
        "POSITION",                   // �Z�}���e�B�N�X
        0,                            // �C���f�b�N�X
        DXGI_FORMAT_R32G32B32_FLOAT,  // �t�H�[�}�b�g
        0,                            // ���̓A�Z���u���[���ʎq(?)
        0,                            // �I�t�Z�b�g�i�o�C�g�j�i�ȗ����j
        D3D11_INPUT_PER_VERTEX_DATA,  // ���̓f�[�^�N���X����
        0                             // �X�e�b�v���[�g
      },
  };
  ULONG layout_count = sizeof(layout) / sizeof(layout[0]);

  // ���̓��C�A�E�g����
  HRESULT ret = device->CreateInputLayout(elem_layout,
    layout_count,
    vertex_shader_data.GetPointer(),
    vertex_shader_data.GetSize(),
    &layout);
  vertex_shader_data.Free();

  if (FAILED(ret)) { return false; }

  // ���̓A�Z���u���Ƀ��C�A�E�g�ݒ�
  device_context->IASetInputLayout(layout);

  return true;
}

bool CreateVertexBuffers(ID3D11Device* device) {
  // ���_�̒�`
  float vertexes[] = {
      0.f, 0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f,
  };

  // ���_�o�b�t�@�ݒ�
  D3D11_BUFFER_DESC buffer_describe = { 0 };
  buffer_describe.ByteWidth = sizeof(vertexes);
  buffer_describe.Usage = D3D11_USAGE_DEFAULT;
  buffer_describe.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  buffer_describe.CPUAccessFlags = 0;
  buffer_describe.MiscFlags = 0;
  buffer_describe.StructureByteStride = 0;

  // �T�u���\�[�X�̐ݒ�
  D3D11_SUBRESOURCE_DATA subresource = { 0 };
  subresource.pSysMem = vertexes;
  subresource.SysMemPitch = 0;
  subresource.SysMemSlicePitch = 0;

  // ���_�o�b�t�@�쐬
  HRESULT ret = device->CreateBuffer(&buffer_describe, &subresource, &vertex_buffer);
  return SUCCEEDED(ret);
}

/// @fn DirectX11Module
/// @brief �R���X�g���N�^
/// @param[in] window_handle �E�B���h�E�n���h��
DirectX11Module::DirectX11Module(HWND window_handle) :
ReferenceCountInterface("dx11module"),
_window_handle(window_handle),
_device(nullptr),
_device_context(nullptr),
_factory(nullptr) {

  // �t�@�N�g���[�쐬
  CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&_factory));

  _CreateDevice();

  _swap_chain = CreateSwapChain(window_handle);

  //LoadToCreateVertexShader(_device);

  //LoadToCreateGeometryShader(_device);

  //LoadToCreatePixelShader(_device);

  //CreateVertexBuffers(_device);

  //CreateInputLayout(_device, _device_context);

}

/// @fn ~DirectX11Module
/// @brief �f�X�g���N�^
DirectX11Module::~DirectX11Module() {
  _swap_chain->Release();
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

/// @fn Start
/// @brief �n�߂�
void DirectX11Module::Start() {
}

void DirectX11Module::render() {
  // �����_�[�^�[�Q�b�g�r���[���N���A
  float clear_color[4] = { 0.3f, 0.3f, 1.f, 1.f };
  _device_context->ClearRenderTargetView(_swap_chain->_render_target_view, clear_color);

  // �V�F�[�_��ݒ肵�ĕ`��
  //_device_context->VSSetShader(vertex_shader, nullptr, 0);
  //_device_context->GSSetShader(geometry_shader, nullptr, 0);
  //_device_context->PSSetShader(pixel_shader, nullptr, 0);

  // ���̓A�Z���u���ɒ��_�o�b�t�@�ݒ�
  UINT stride = sizeof(float)*3;
  UINT offset = 0;
  //_device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

  // �v���~�e�B�u�̎�ނ�ݒ�
  //_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //_device_context->Draw(3, 0);

  // �t���b�v
  _swap_chain->Present();
}

//EOF