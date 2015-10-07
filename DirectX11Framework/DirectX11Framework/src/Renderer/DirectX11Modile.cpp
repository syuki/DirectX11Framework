/// @file DirectX11Module.cpp
/// @brief DirectX11モジュール
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
/// @brief DirectX11Module作成
/// @param[in] window_handle ウィンドウハンドル
DirectX11Module* DirectX11Module::CreateDirectX11Module(HWND window_handle) {
  return new DirectX11Module(window_handle);
}

/// @class RawFile
class RawData {
public:

  /// @fn RawData
  /// @brief デフォルトコンストラクタ
  RawData() : pointer_(nullptr), size_(0) {
  }

  /// @fn RawData
  /// @brief ムーブコンストラクタ
  /// @param[in] src 移動元
  RawData(RawData&& src)
    : pointer_(src.pointer_),
    size_(src.size_) {
    src.pointer_ = nullptr;
    src.size_ = 0;
  }

  /// @fn RawData
  /// @brief ファイルを指定して読み込む
  /// @param[in] filename ファイル名
  RawData(const char* filename) {
    LoadFromFile(filename);
  }

  /// @fn RawData
  /// @brief ポインタとサイズを渡して読み込む
  /// @param[in] pointer 読み込ませたい
  /// @param[in] size 渡したいサイズ
  RawData(void* pointer, size_t size) {
    LoadFromPointer(pointer_, size);
  }

  /// @fn Allocate
  /// @brief メモリ確保
  /// @param[in] size 確保サイズ
  /// @param[in] force 強行するか
  /// @return 確保できたらtrue
  bool Allocate(size_t size, bool force) {
    _ASSERT_EXPR(size, L"\"RawData\" : allocate size is zero.");
    if (size_ && !force) return false;
    Free();

    size_ = size;
    pointer_ = new char[size_];

    return true;
  }

  /// @fn Free
  /// @brief 確保したメモリ解放
  /// @return 解放できたらtrue
  bool Free() {
    if (!size_) return false;

    delete[] pointer_;
    pointer_ = nullptr;
    size_ = 0;

    return true;
  }

  /// @fn Copy
  /// @brief 引数のメモリをコピーする
  ///        メモリが確保してある場合はそのまま使う
  ///        足らない場合はこの関数は失敗する
  /// @param[in] source コピーソース
  /// @param[in] size サイズ
  /// @param[in] clear メモリ内をクリアするか
  /// @return コピーできたらture
  bool Copy(void* source, size_t size, bool clear = true) {
    if (size > size_) return false;

    if (clear) {
      ZeroMemory(pointer_, size_);
    }
    memcpy_s(pointer_, size_, source, size);

    return true;
  }

  /// @fn LoadFromFile
  /// @brief ファイルから読み込み
  /// @param[in] filename ファイル名
  /// @return 読み込めたらtrue
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
  /// @brief ポインタから読み込む
  ///        メモリが確保されている場合は破棄して取り直す
  /// @param[in] pointer 読みたいポインタ
  /// @param[in] size 読ませたいサイズ
  /// @param[in] force 強行するか
  /// @return 読み込めたらtrue
  bool RawData::LoadFromPointer(void* pointer, size_t size, bool force = false) {
    if (!Allocate(size, true)) return false;
    return Copy(pointer, size);
  }

  /// @fn GetPointer
  /// @brief ポインタ取得
  /// @return ポインタ
  __forceinline void* GetPointer() { return pointer_; }

  /// @fn GetSize
  /// @brief サイズ取得
  /// @return サイズ
  __forceinline size_t GetSize() { return size_; }

  /// @fn Empty
  /// @brief 値が入っていないか
  /// @return 空ならtrue
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
_window_handle(window_handle) {

  // クライアント領域サイズ取得
  RECT client_rect;
  GetClientRect(window_handle, &client_rect);

  // バッファサイズ設定
  ULONG width = client_rect.right - client_rect.left;
  ULONG height = client_rect.bottom - client_rect.top;

  // デバイスフラグ設定
  UINT device_frag = 0;
#ifdef _DEBUG
  device_frag = D3D11_CREATE_DEVICE_DEBUG;
#endif

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

  // ファクトリー作成
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

  // 成功するまでループをやめない
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
  _ASSERT_EXPR(SUCCEEDED(ret), L"デバイスの作成に失敗");

  // スワップチェイン設定
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

  // スワップチェイン作成
  factory->CreateSwapChain(_device, &swap_chain, &_swap_chain);
  factory->Release();
  factory = nullptr;

  // バックバッファ取得
  ID3D11Texture2D* back_buffer_ = nullptr;
  ret = _swap_chain->GetBuffer(0,
    __uuidof(ID3D11Texture2D),
    (void**)&back_buffer_);
  _ASSERT_EXPR(SUCCEEDED(ret), L"バッファ取得失敗");

  // レンダーターゲットビュー生成
  ret = _device->CreateRenderTargetView(back_buffer_,
    nullptr,
    &_render_target_view);
  back_buffer_->Release();
  back_buffer_ = nullptr;
  _ASSERT_EXPR(SUCCEEDED(ret), L"レンダーターゲット作成失敗");

  // 出力マネージャにレンダーターゲットビュー設定
  _device_context->OMSetRenderTargets(1,
    &_render_target_view,
    nullptr);

  // ビューポート設定
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
/// @brief デストラクタ
DirectX11Module::~DirectX11Module() {
}


void DirectX11Module::render() {
  // レンダーターゲットビューをクリア
  float clear_color[4] = { 0.3f, 0.3f, 1.f, 1.f };
  _device_context->ClearRenderTargetView(_render_target_view, clear_color);

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
  _swap_chain->Present(0, 0);
}

//EOF