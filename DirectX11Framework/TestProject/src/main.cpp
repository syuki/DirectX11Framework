#include <Windows.h>
#include "module/WindowModule.h"
#include <Renderer/DirectX11Module.h>
#include <Util/Math.h>
#include <Renderer/SwapChain.h>
#include <Shader/VertexShader.h>
#include <Shader/PixelShader.h>
#include <Shader/GeometryShader.h>
#include <Shader/InputLayout.h>
#include <Util/DataContainer.h>
#include <Buffer/VertexBuffer.h>

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

  auto right_window = new WindowModule(L"Right", 450, 800, ins);
  right_window->Initialize();
  right_window->SetMessageCallback(WM_DESTROY, [](HWND, WPARAM, LPARAM) {
    PostQuitMessage(0);
  });
  right_window->SetMessageCallback(WM_KEYDOWN, [](HWND h, WPARAM w, LPARAM) {
    if (w == VK_ESCAPE) { DestroyWindow(h); }
  });

  // レンダラ作成
  auto renderer = DirectX11Module::CreateDirectX11Module();
  auto device = renderer->GetDevice();
  auto context = renderer->GetDeviceContext();

  // スワップチェイン作成
  auto left_swap_chain = renderer->CreateSwapChain(left_window->GetWindowHandle());
  auto right_swap_chain = renderer->CreateSwapChain(right_window->GetWindowHandle());

  // データ作成
  auto vs_data = DataContainer::Get("res/shader/VertexShader.cso");
  auto ps_data = DataContainer::Get("res/shader/PixelShader.cso");
  auto gs_data = DataContainer::Get("res/shader/GeometryShader.cso");

  // シェーダ作成
  D3D11_INPUT_ELEMENT_DESC elem_desc[] = {
      {
        "POSITION",                   // セマンティクス
        0,                            // インデックス5
        DXGI_FORMAT_R32G32B32_FLOAT,  // フォーマット
        0,                            // 入力アセンブラー識別子(?)
        0,                            // オフセット（バイト）（省略化）
        D3D11_INPUT_PER_VERTEX_DATA,  // 入力データクラス識別
        0                             // ステップレート
      },
  };
  ULONG elem_count = sizeof(elem_desc) / sizeof(elem_desc[0]);
  auto vertex_shader = new VertexShader(device, vs_data);
  auto pixel_shader = new PixelShader(device, ps_data);
  auto geometry_shader = new GeometryShader(device, gs_data);
  auto input_layout = new InputLayout(device, vs_data, elem_desc, elem_count);

  // シェーダ適応
  vertex_shader->SetVertexShader(context);
  pixel_shader->SetPixelShader(context);
  geometry_shader->SetGeometryShader(context);
  input_layout->SetInputLayout(context);

  // 頂点バッファ作成
  float vertexes[] = {
    0.f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
  };
  auto vertex_buffer = new VertexBuffer(device, vertexes, sizeof(vertexes), sizeof(float) * 3);

  // ビューポート設定
  D3D11_VIEWPORT view_port;
  view_port.Width = static_cast<float>(800);
  view_port.Height = static_cast<float>(600);
  view_port.MinDepth = 0.f;
  view_port.MaxDepth = 1.f;
  view_port.TopLeftX = 0.f;
  view_port.TopLeftY = 0.f;
  context->RSSetViewports(1, &view_port);

  // メッセージループ
  while (left_window->MessageProcessing()) {
    float clear_color[4] = { 1, 0, 1, 1 };
    auto l_rtv = left_swap_chain->GetRenderTargetView();
    auto r_rtv = right_swap_chain->GetRenderTargetView();

    context->ClearRenderTargetView(l_rtv, clear_color);

    context->OMSetRenderTargets(1, &l_rtv, nullptr);

    vertex_buffer->SetVertexBuffer(renderer->GetDeviceContext(), 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    renderer->GetDeviceContext()->Draw(3, 0);

    left_swap_chain->Present();

    context->ClearRenderTargetView(r_rtv, clear_color);

    context->OMSetRenderTargets(1, &r_rtv, nullptr);

    right_swap_chain->Present();
  }
  delete vertex_buffer;
  delete input_layout;
  delete geometry_shader;
  delete pixel_shader;
  delete vertex_shader;

  DataContainer::Unregist("res/shader/vertex.cso");
  DataContainer::Unregist("res/shader/pixel.cso");
  DataContainer::Unregist("res/shader/geometry.cso");

  right_swap_chain->Release();
  left_swap_chain->Release();
  renderer->Release();

  // 終了
  left_window->Finalize();
  right_window->Finalize();
  delete left_window;
  delete right_window;

  return 0;
}