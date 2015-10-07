/// @file DirectX11Module.cpp
/// @brief DirectX11���W���[��
/// @date 2015/10/02
/// @author syuki nishida

#include "Renderer/DirectX11Module.h"
#include <vector>
#include <crtdbg.h>
#include <errno.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment(lib, "dxgi.lib")

using namespace snlib;

/// @fn createDirectX11Module
/// @brief DirectX11Module�쐬
/// @param[in] window_handle �E�B���h�E�n���h��
DirectX11Module* DirectX11Module::CreateDirectX11Module(HWND window_handle) {
  return new DirectX11Module(window_handle);
}

/// @class RawFile
class RawData {
public:

  /// @fn RawData
  /// @brief �f�t�H���g�R���X�g���N�^
  RawData() : pointer_(nullptr), size_(0) {
  }

  /// @fn RawData
  /// @brief ���[�u�R���X�g���N�^
  /// @param[in] src �ړ���
  RawData(RawData&& src)
    : pointer_(src.pointer_),
    size_(src.size_) {
    src.pointer_ = nullptr;
    src.size_ = 0;
  }

  /// @fn RawData
  /// @brief �t�@�C�����w�肵�ēǂݍ���
  /// @param[in] filename �t�@�C����
  RawData(const char* filename) {
    LoadFromFile(filename);
  }

  /// @fn RawData
  /// @brief �|�C���^�ƃT�C�Y��n���ēǂݍ���
  /// @param[in] pointer �ǂݍ��܂�����
  /// @param[in] size �n�������T�C�Y
  RawData(void* pointer, size_t size) {
    LoadFromPointer(pointer_, size);
  }

  /// @fn Allocate
  /// @brief �������m��
  /// @param[in] size �m�ۃT�C�Y
  /// @param[in] force ���s���邩
  /// @return �m�ۂł�����true
  bool Allocate(size_t size, bool force) {
    _ASSERT_EXPR(size, L"\"RawData\" : allocate size is zero.");
    if (size_ && !force) return false;
    Free();

    size_ = size;
    pointer_ = new char[size_];

    return true;
  }

  /// @fn Free
  /// @brief �m�ۂ������������
  /// @return ����ł�����true
  bool Free() {
    if (!size_) return false;

    delete[] pointer_;
    pointer_ = nullptr;
    size_ = 0;

    return true;
  }

  /// @fn Copy
  /// @brief �����̃��������R�s�[����
  ///        ���������m�ۂ��Ă���ꍇ�͂��̂܂܎g��
  ///        ����Ȃ��ꍇ�͂��̊֐��͎��s����
  /// @param[in] source �R�s�[�\�[�X
  /// @param[in] size �T�C�Y
  /// @param[in] clear �����������N���A���邩
  /// @return �R�s�[�ł�����ture
  bool Copy(void* source, size_t size, bool clear = true) {
    if (size > size_) return false;

    if (clear) {
      ZeroMemory(pointer_, size_);
    }
    memcpy_s(pointer_, size_, source, size);

    return true;
  }

  /// @fn LoadFromFile
  /// @brief �t�@�C������ǂݍ���
  /// @param[in] filename �t�@�C����
  /// @return �ǂݍ��߂���true
  bool LoadFromFile(const char* filename) {
    FILE* fp;
    int ret = fopen_s(&fp, filename, "rb");
    if (ret == EINVAL) return false;

    fpos_t file_size;
    fseek(fp, 0, SEEK_END);
    fgetpos(fp, &file_size);
    fseek(fp, 0, SEEK_SET);
    if (file_size > UINT_MAX || !file_size) {
      _ASSERT_EXPR(file_size <= UINT_MAX, L"\"RawData\" : file size is big.");
      _ASSERT_EXPR(file_size, L"\"RawData\" : file size is zero.");
      fclose(fp);
      return false;
    }

    size_ = static_cast<size_t>(file_size);
    char* buffer = new char[size_];
    fread_s(buffer, size_, sizeof(char), size_, fp);

    Allocate(size_, true);
    Copy(buffer, size_);

    return true;
  }

  /// @fn LoadFromPointer
  /// @brief �|�C���^����ǂݍ���
  ///        ���������m�ۂ���Ă���ꍇ�͔j�����Ď�蒼��
  /// @param[in] pointer �ǂ݂����|�C���^
  /// @param[in] size �ǂ܂������T�C�Y
  /// @param[in] force ���s���邩
  /// @return �ǂݍ��߂���true
  bool RawData::LoadFromPointer(void* pointer, size_t size, bool force = false) {
    if (!Allocate(size, true)) return false;
    return Copy(pointer, size);
  }

  /// @fn GetPointer
  /// @brief �|�C���^�擾
  /// @return �|�C���^
  __forceinline void* GetPointer() { return pointer_; }

  /// @fn GetSize
  /// @brief �T�C�Y�擾
  /// @return �T�C�Y
  __forceinline size_t GetSize() { return size_; }

  /// @fn Empty
  /// @brief �l�������Ă��Ȃ���
  /// @return ��Ȃ�true
  __forceinline bool Empty() { return pointer_ ? false : true; }

protected:

private:

  void* pointer_;
  size_t size_;
};

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
_window_handle(window_handle) {

  // �N���C�A���g�̈�T�C�Y�擾
  RECT client_rect;
  GetClientRect(window_handle, &client_rect);

  // �o�b�t�@�T�C�Y�ݒ�
  ULONG width = client_rect.right - client_rect.left;
  ULONG height = client_rect.bottom - client_rect.top;

  // �f�o�C�X�t���O�ݒ�
  UINT device_frag = 0;
#ifdef _DEBUG
  device_frag = D3D11_CREATE_DEVICE_DEBUG;
#endif

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

  // �t�@�N�g���[�쐬
  IDXGIFactory1* factory;
  CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory));

  IDXGIAdapter1* adapter;
  for (UINT index = 0;; index++) {
    HRESULT ret = factory->EnumAdapters1(index, &adapter);
    if (ret == DXGI_ERROR_NOT_FOUND) {
      break;
    }
    adapter->Release();
  }

  // ��������܂Ń��[�v����߂Ȃ�
  HRESULT ret;
  for (ULONG i = 0; i < driver_type_count; i++) {
    ret = D3D11CreateDevice(
      adapter,
      adapter ? D3D_DRIVER_TYPE_UNKNOWN : driver_type[i],
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
  _ASSERT_EXPR(SUCCEEDED(ret), L"�f�o�C�X�̍쐬�Ɏ��s");

  // �X���b�v�`�F�C���ݒ�
  DXGI_SWAP_CHAIN_DESC swap_chain = { 0 };
  swap_chain.BufferDesc.Width = width;
  swap_chain.BufferDesc.Height = height;
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

  // �X���b�v�`�F�C���쐬
  factory->CreateSwapChain(_device, &swap_chain, &_swap_chain);
  factory->Release();
  factory = nullptr;

  // �o�b�N�o�b�t�@�擾
  ID3D11Texture2D* back_buffer_ = nullptr;
  ret = _swap_chain->GetBuffer(0,
    __uuidof(ID3D11Texture2D),
    (void**)&back_buffer_);
  _ASSERT_EXPR(SUCCEEDED(ret), L"�o�b�t�@�擾���s");

  // �����_�[�^�[�Q�b�g�r���[����
  ret = _device->CreateRenderTargetView(back_buffer_,
    nullptr,
    &_render_target_view);
  back_buffer_->Release();
  back_buffer_ = nullptr;
  _ASSERT_EXPR(SUCCEEDED(ret), L"�����_�[�^�[�Q�b�g�쐬���s");

  // �o�̓}�l�[�W���Ƀ����_�[�^�[�Q�b�g�r���[�ݒ�
  _device_context->OMSetRenderTargets(1,
    &_render_target_view,
    nullptr);

  // �r���[�|�[�g�ݒ�
  D3D11_VIEWPORT view_port;
  view_port.Width = static_cast<float>(width);
  view_port.Height = static_cast<float>(height);
  view_port.MinDepth = 0.f;
  view_port.MaxDepth = 1.f;
  view_port.TopLeftX = 0.f;
  view_port.TopLeftY = 0.f;
  _device_context->RSSetViewports(1, &view_port);

  //LoadToCreateVertexShader(_device);

  //LoadToCreateGeometryShader(_device);

  //LoadToCreatePixelShader(_device);

  //CreateVertexBuffers(_device);

  //CreateInputLayout(_device, _device_context);

}

/// @fn ~DirectX11Module
/// @brief �f�X�g���N�^
DirectX11Module::~DirectX11Module() {
}


void DirectX11Module::render() {
  // �����_�[�^�[�Q�b�g�r���[���N���A
  float clear_color[4] = { 0.3f, 0.3f, 1.f, 1.f };
  _device_context->ClearRenderTargetView(_render_target_view, clear_color);

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
  _swap_chain->Present(0, 0);
}

//EOF