#include <Windows.h>
#include "module/WindowModule.h"
#include <Renderer/Device.h>
#include <Renderer/Context.h>
#include <Renderer/DxGiAdapter.h>
#include <Util/Math.h>
#include <Renderer/SwapChain.h>
#include <Shader/VertexShader.h>
#include <Shader/PixelShader.h>
#include <Shader/GeometryShader.h>
#include <Shader/InputLayout.h>
#include <Util/DataContainer.h>
#include <Buffer/VertexBuffer.h>
#include <Util/Math.h>
#include <Util/FrameKeeper.h>

#pragma comment(lib, "DirectX11Framework.lib")

using namespace snlib;

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

int WINAPI WinMain(HINSTANCE ins, HINSTANCE prev, LPSTR cmd, int show) {

  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  // ウィンドウ生成
  auto* left_window = new WindowModule(L"Left", 800, 600, ins);
  left_window->Initialize();
  left_window->SetMessageCallback(WM_DESTROY, [](HWND, WPARAM, LPARAM) {
    PostQuitMessage(0);
  });
  left_window->SetMessageCallback(WM_KEYDOWN, [](HWND h, WPARAM w, LPARAM) {
    if (w == VK_ESCAPE) { DestroyWindow(h); }
  });

  //auto right_window = new WindowModule(L"Right", 450, 800, ins);
  //right_window->Initialize();
  //right_window->SetMessageCallback(WM_DESTROY, [](HWND, WPARAM, LPARAM) {
  //  PostQuitMessage(0);
  //});
  //right_window->SetMessageCallback(WM_KEYDOWN, [](HWND h, WPARAM w, LPARAM) {
  //  if (w == VK_ESCAPE) { DestroyWindow(h); }
  //});

  // レンダラ作成
  auto my_device = CreateDevice();
  auto my_context = my_device->GetContext();
  auto my_dxgi = CreateDxGiAdapter();
  auto device = my_device->GetDevice();
  auto context = my_context->GetContext();

  // スワップチェイン作成
  auto left_swap_chain = my_dxgi->CreateSwapChain(my_device->GetDevice(),left_window->GetWindowHandle());
  //auto right_swap_chain = my_dxgi->CreateSwapChain(my_device->GetDevice(),right_window->GetWindowHandle());

  // データ作成
  auto vs_data = DataContainer::Get("res/shader/VertexShader.cso");
  auto ps_data = DataContainer::Get("res/shader/PixelShader.cso");

  // シェーダ作成
  D3D11_INPUT_ELEMENT_DESC elem_desc[] = {
      {
        "POSITION",                   // セマンティクス
        0,                            // インデックス5
        DXGI_FORMAT_R32G32B32_FLOAT,  // フォーマット
        1,                            // 入力アセンブラー識別子(?)
        0,                            // オフセット（バイト）（省略化）
        D3D11_INPUT_PER_VERTEX_DATA,  // 入力データクラス識別
        0                             // ステップレート
      },
      {
        "COLOR",                      // セマンティクス
        0,                            // インデックス5
        DXGI_FORMAT_R32G32B32A32_FLOAT,  // フォーマット
        1,                            // 入力アセンブラー識別子(?)
        4*3,                            // オフセット（バイト）（省略化）
        D3D11_INPUT_PER_VERTEX_DATA,  // 入力データクラス識別
        0                             // ステップレート
      },
  };
  ULONG elem_count = sizeof(elem_desc) / sizeof(elem_desc[0]);
  auto vertex_shader = new VertexShader(device, vs_data);
  auto pixel_shader = new PixelShader(device, ps_data);
  auto input_layout = new InputLayout(device, vs_data, elem_desc, elem_count);

  // シェーダ適応
  vertex_shader->SetVertexShader(context);
  pixel_shader->SetPixelShader(context);
  input_layout->SetInputLayout(context);

  // 頂点バッファ作成
  float vertexes[] = {
    // fu
    -0.5f, +0.5f, -0.5f, 1, 0, 0, 1,
    +0.5f, +0.5f, -0.5f, 1, 0, 0, 1,
    -0.5f, -0.5f, -0.5f, 1, 0, 0, 1,

    // fd
    -0.5f, -0.5f, -0.5f, 1, 0, 0, 1,
    +0.5f, +0.5f, -0.5f, 1, 0, 0, 1,
    +0.5f, -0.5f, -0.5f, 1, 0, 0, 1,

    // bu
    +0.5f, +0.5f, +0.5f, 1, 0, 0, 1,
    -0.5f, +0.5f, +0.5f, 1, 0, 0, 1,
    +0.5f, -0.5f, +0.5f, 1, 0, 0, 1,

    // bd
    +0.5f, -0.5f, +0.5f, 1, 0, 0, 1,
    -0.5f, +0.5f, +0.5f, 1, 0, 0, 1,
    -0.5f, -0.5f, +0.5f, 1, 0, 0, 1,

    // lu
    -0.5f, +0.5f, +0.5f, 0, 0, 1, 1,
    -0.5f, +0.5f, -0.5f, 0, 0, 1, 1,
    -0.5f, -0.5f, +0.5f, 0, 0, 1, 1,

    // ld
    -0.5f, -0.5f, +0.5f, 0, 0, 1, 1,
    -0.5f, +0.5f, -0.5f, 0, 0, 1, 1,
    -0.5f, -0.5f, -0.5f, 0, 0, 1, 1,

    // ru
    +0.5f, +0.5f, -0.5f, 0, 0, 1, 1,
    +0.5f, +0.5f, +0.5f, 0, 0, 1, 1,
    +0.5f, -0.5f, -0.5f, 0, 0, 1, 1,

    // rd
    +0.5f, -0.5f, -0.5f, 0, 0, 1, 1,
    +0.5f, +0.5f, +0.5f, 0, 0, 1, 1,
    +0.5f, -0.5f, +0.5f, 0, 0, 1, 1,

  };
  auto vertex_buffer = new VertexBuffer(device, vertexes, sizeof(vertexes), sizeof(float) * 7);

  // ビューポート設定
  D3D11_VIEWPORT view_port;
  view_port.Width = static_cast<float>(1440);
  view_port.Height = static_cast<float>(900);
  view_port.MinDepth = 0.f;
  view_port.MaxDepth = 1.f;
  view_port.TopLeftX = 0.f;
  view_port.TopLeftY = 0.f;
  context->RSSetViewports(1, &view_port);

  auto adapter = my_dxgi->GetAdapter();
  IDXGIOutput* output;
  adapter->EnumOutputs(0, &output);

  //left_swap_chain->ResizeBuffer(output);
  //left_swap_chain->SetOutput(output, true);
  //left_swap_chain->ResizeBuffer(output, true);
  output->Release();

  struct ConstantBuffer {
    Matrix world;
    Matrix view;
    Matrix projection;
  };
  ConstantBuffer constant_buffer_data;

  D3D11_BUFFER_DESC constant_buffer_desc = { 0 };
  constant_buffer_desc.ByteWidth = sizeof(constant_buffer_data);
  constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constant_buffer_desc.CPUAccessFlags = 0;
  constant_buffer_desc.MiscFlags = 0;
  constant_buffer_desc.StructureByteStride = sizeof(float);

  ID3D11Buffer* constant_buffer;
  HRESULT ret;
  ret = device->CreateBuffer(&constant_buffer_desc, nullptr, &constant_buffer);
  _ASSERT_EXPR(SUCCEEDED(ret), L"コンスタントバッファ作成失敗");

  float ff = 0;
  FrameKeeper fk(60, [&]{
    left_swap_chain->Clear(context);
    left_swap_chain->SetRenderTarget(context);

    ff += 0.1f;
    constant_buffer_data.world.SetScale(10);
    constant_buffer_data.world.SetIdentity();
    constant_buffer_data.world.RotateY(-ff);
    constant_buffer_data.view.SetView(Vector3(0, 0, -10), Vector3(0, 0, 1), Vector3(0, 1, 0));
    constant_buffer_data.projection.PerthFovL(PI / 8.f, 4.f / 3.f, 0.1f, 1000.f);
    context->UpdateSubresource(constant_buffer, 0, nullptr, &constant_buffer_data, 0, 0);
    context->VSSetConstantBuffers(0, 1, &constant_buffer);

    vertex_buffer->SetVertexBuffer(context, 1);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->Draw(24, 0);

    left_swap_chain->Present();

    //right_swap_chain->Clear(context);
    //right_swap_chain->SetRenderTarget(context);

    //right_swap_chain->Present();
  });

  // メッセージループ
  constant_buffer_data.world.SetIdentity();
  while (left_window->MessageProcessing()) {
    fk.Step();
  }

  constant_buffer->Release();

  delete vertex_buffer;
  delete input_layout;
  delete pixel_shader;
  delete vertex_shader;

  DataContainer::Unregist("res/shader/vertex.cso");
  DataContainer::Unregist("res/shader/pixel.cso");
  DataContainer::Unregist("res/shader/geometry.cso");

  //right_swap_chain->Release();
  left_swap_chain->Release();
  my_dxgi->Release();
  my_context->Release();
  my_device->Release();

  // 終了
  left_window->Finalize();
  //right_window->Finalize();
  delete left_window;
  //delete right_window;

  return 0;
}