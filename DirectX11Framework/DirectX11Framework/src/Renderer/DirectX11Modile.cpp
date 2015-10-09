/// @file DirectX11Module.cpp
/// @brief DirectX11モジュール
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
/// @brief DirectX11Module作成
/// @param[in] window_handle ウィンドウハンドル
DirectX11Module* DirectX11Module::CreateDirectX11Module(HWND window_handle) {
  return new DirectX11Module(window_handle);
}

/// @fn CreateSwapChain
/// @brief スワップチェイン作成
SwapChain* DirectX11Module::CreateSwapChain(HWND window_handle) {
  return new SwapChain(_factory, _device, window_handle);
}

ID3D11VertexShader* vertex_shader;
ID3D11PixelShader* pixel_shader;
ID3D11GeometryShader* geometry_shader;
ID3D11InputLayout* layout;
ID3D11Buffer* vertex_buffer;

bool LoadToCreateVertexShader(ID3D11Device* device) {
  // 頂点シェーダ読み込み
  RawData vertex_shader_data("data/shader/vertex.cso");
  if (vertex_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"vs load error");
    return false;
  }

  // 頂点シェーダ作成
  HRESULT ret = device->CreateVertexShader(vertex_shader_data.GetPointer(),
    vertex_shader_data.GetSize(),
    nullptr,
    &vertex_shader);
  vertex_shader_data.Free();

  return SUCCEEDED(ret);
}

bool LoadToCreatePixelShader(ID3D11Device* device) {
  // ピクセルシェーダ読み込み
  RawData pixel_shader_data("data/shader/pixel.cso");
  if (pixel_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"ps load error");
    return false;
  }

  // ピクセルシェーダ作成
  HRESULT ret = device->CreatePixelShader(pixel_shader_data.GetPointer(),
    pixel_shader_data.GetSize(),
    nullptr,
    &pixel_shader);
  pixel_shader_data.Free();

  return SUCCEEDED(ret);
}

bool LoadToCreateGeometryShader(ID3D11Device* device) {
  // ジオメトリシェーダ読み込み
  RawData geometry_shader_data("data/shader/geometry.cso");
  if (geometry_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"gs load error");
    return false;
  }

  // ジオメトリシェーダ作成
  HRESULT ret = device->CreateGeometryShader(geometry_shader_data.GetPointer(),
    geometry_shader_data.GetSize(),
    nullptr,
    &geometry_shader);
  geometry_shader_data.Free();

  return SUCCEEDED(ret);
}

bool CreateInputLayout(ID3D11Device* device, ID3D11DeviceContext* device_context) {
  // 頂点シェーダ読み込み
  RawData vertex_shader_data("data/shader/vertex.cso");
  if (vertex_shader_data.Empty()) {
    _ASSERT_EXPR(0, L"vs load error");
    return false;
  }

  // 入力レイアウト定義
  D3D11_INPUT_ELEMENT_DESC elem_layout[] = {
      {
        "POSITION",                   // セマンティクス
        0,                            // インデックス
        DXGI_FORMAT_R32G32B32_FLOAT,  // フォーマット
        0,                            // 入力アセンブラー識別子(?)
        0,                            // オフセット（バイト）（省略化）
        D3D11_INPUT_PER_VERTEX_DATA,  // 入力データクラス識別
        0                             // ステップレート
      },
  };
  ULONG layout_count = sizeof(layout) / sizeof(layout[0]);

  // 入力レイアウト生成
  HRESULT ret = device->CreateInputLayout(elem_layout,
    layout_count,
    vertex_shader_data.GetPointer(),
    vertex_shader_data.GetSize(),
    &layout);
  vertex_shader_data.Free();

  if (FAILED(ret)) { return false; }

  // 入力アセンブラにレイアウト設定
  device_context->IASetInputLayout(layout);

  return true;
}

bool CreateVertexBuffers(ID3D11Device* device) {
  // 頂点の定義
  float vertexes[] = {
      0.f, 0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f,
  };

  // 頂点バッファ設定
  D3D11_BUFFER_DESC buffer_describe = { 0 };
  buffer_describe.ByteWidth = sizeof(vertexes);
  buffer_describe.Usage = D3D11_USAGE_DEFAULT;
  buffer_describe.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  buffer_describe.CPUAccessFlags = 0;
  buffer_describe.MiscFlags = 0;
  buffer_describe.StructureByteStride = 0;

  // サブリソースの設定
  D3D11_SUBRESOURCE_DATA subresource = { 0 };
  subresource.pSysMem = vertexes;
  subresource.SysMemPitch = 0;
  subresource.SysMemSlicePitch = 0;

  // 頂点バッファ作成
  HRESULT ret = device->CreateBuffer(&buffer_describe, &subresource, &vertex_buffer);
  return SUCCEEDED(ret);
}

/// @fn DirectX11Module
/// @brief コンストラクタ
/// @param[in] window_handle ウィンドウハンドル
DirectX11Module::DirectX11Module(HWND window_handle) :
ReferenceCountInterface("dx11module"),
_window_handle(window_handle),
_device(nullptr),
_device_context(nullptr),
_factory(nullptr) {

  // ファクトリー作成
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
/// @brief デストラクタ
DirectX11Module::~DirectX11Module() {
  _swap_chain->Release();
  _DeleteDevice();
  SafeRelease(_factory);
}

/// @fn _CreateDevice
/// @brief デバイス作成
/// @param[in] adapter アダプタ
void DirectX11Module::_CreateDevice(IDXGIAdapter1* adapter) {

  _ASSERT_EXPR(!_device, L"DirectX11Framework : デバイスが既にあります");
  _ASSERT_EXPR(!_device_context, L"DirectX11Framework : Contextが既にあります");

  // ドライバタイプ設定
  D3D_DRIVER_TYPE driver_type[] = {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  ULONG driver_type_count = sizeof(driver_type) / sizeof(D3D_DRIVER_TYPE);

  // 機能レベル設定
  D3D_FEATURE_LEVEL feature_level[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  ULONG feature_level_count = sizeof(feature_level) / sizeof(D3D_FEATURE_LEVEL);

  // デバイスフラグ設定
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
  _ASSERT_EXPR(SUCCEEDED(ret), L"DirectX11Framework : デバイスの作成に失敗");
}

/// @fn DeleteDevice
/// @brief デバイス削除
void DirectX11Module::_DeleteDevice() {
  SafeRelease(_device_context);
  SafeRelease(_device);
}

/// @fn Start
/// @brief 始める
void DirectX11Module::Start() {
}

void DirectX11Module::render() {
  // レンダーターゲットビューをクリア
  float clear_color[4] = { 0.3f, 0.3f, 1.f, 1.f };
  _device_context->ClearRenderTargetView(_swap_chain->_render_target_view, clear_color);

  // シェーダを設定して描画
  //_device_context->VSSetShader(vertex_shader, nullptr, 0);
  //_device_context->GSSetShader(geometry_shader, nullptr, 0);
  //_device_context->PSSetShader(pixel_shader, nullptr, 0);

  // 入力アセンブラに頂点バッファ設定
  UINT stride = sizeof(float)*3;
  UINT offset = 0;
  //_device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

  // プリミティブの種類を設定
  //_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //_device_context->Draw(3, 0);

  // フリップ
  _swap_chain->Present();
}

//EOF