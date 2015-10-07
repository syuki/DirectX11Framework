#include <Windows.h>
#include "module/WindowModule.h"
#include <Renderer/DirectX11Module.h>
#include <Util/Math.h>

#pragma comment(lib, "DirectX11Framework.lib")
using namespace snlib;

int WINAPI WinMain(HINSTANCE ins, HINSTANCE prev, LPSTR cmd, int show) {

  // ウィンドウ生成
  WindowModule* window = new WindowModule(L"TestProject", 800, 600, ins);
  window->Initialize();
  window->SetMessageCallback(WM_DESTROY, [](HWND, WPARAM, LPARAM) {
    PostQuitMessage(0);
  });
  window->SetMessageCallback(WM_KEYDOWN, [](HWND h, WPARAM w, LPARAM) {
    if (w == VK_ESCAPE) { DestroyWindow(h); }
  });

  DirectX11Module* renderer = DirectX11Module::CreateDirectX11Module(window->GetWindowHandle());

  // メッセージループ
  while (window->MessageProcessing()) {
    renderer->render();
  }

  // 終了
  window->Finalize();
  delete window;
  window = nullptr;

  return 0;
}