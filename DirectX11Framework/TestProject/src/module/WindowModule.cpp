/// @file WindowModule.cpp
/// @brief ウィンドウモジュール
/// @data 2015/04/16
/// @author syuki nishida

#include "WindowModule.h"
#include <imm.h>
#pragma comment(lib, "imm32.lib")

/// @namespace snlib
using namespace snlib;

/// @fn WindowModule
/// @biref デフォルトコンストラクタ
WindowModule::WindowModule() :WindowModule(L"") {
}

/// @fn WindowModule
/// @brief キャプションとクライアントサイズ
/// @param[in] caption キャプション
/// @param[in] client_width クライアント幅
/// @param[in] client_height クライアント高さ
WindowModule::WindowModule(LPCWSTR caption, int client_width, int client_height)
  :WindowModule(caption, client_width, client_height, SN_DEFAULT_INSTANCE_HANDLE) {
}

/// @fn WindowModule
/// @brief キャプションとクライントサイズとインスタンスハンドル
/// @param[in] caption キャプション
/// @param[in] client_width クライアント幅
/// @param[in] client_hright クライント高さ
/// @param[in] instance_handle インスタンスハンドル
WindowModule::WindowModule(LPCWSTR caption, int client_width, int client_height,
  HINSTANCE instance_handle)
  :WindowModule(
    caption,
    caption,
    SN_DEFAULT_CLASS_STYLE,
    instance_handle,
    nullptr,
    nullptr,
    SN_DEFAULT_MOUSE_CURSOR,
    SN_DEFAULT_BACKGROUND_COLOR,
    0,
    nullptr,
    SN_DEFAULT_WINDOW_STYLE,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    client_width,
    client_height,
    nullptr) {
}

/// @fn WindowModule
/// @brief 全引数付きコンストラクタ
WindowModule::WindowModule(
  LPCWSTR class_name,
  LPCWSTR caption,
  UINT class_style,
  HINSTANCE instance_handle,
  WNDPROC procedure,
  HICON icon_handle,
  HCURSOR cursor_handle,
  HBRUSH background_color,
  UINT menu_id,
  HICON mini_icon_handle,
  DWORD window_style,
  int position_x,
  int position_y,
  int client_width,
  int client_height,
  HMENU menu_handle
  )
  :window_class_name_(class_name),
   menu_id_(menu_id),
   caption_(caption),
   window_style_(window_style),
   position_x_(position_x),
   position_y_(position_y),
   client_width_(client_width),
   client_height_(client_height),
   non_client_width_(0),
   non_client_height_(0),
   menu_handle_(menu_handle),
   window_handle_(nullptr),
   instance_handle_(instance_handle) {

  auto window_proc = [](HWND hWnd, UINT uMsg, WPARAM wPar, LPARAM lPar)->LRESULT {
    static std::map<UINT, std::vector<std::function<void(HWND,WPARAM&,LPARAM&)>>>* callbacks = nullptr;
    static bool create_ = false;
    static WPARAM wp = 0;
    static LPARAM lp = 0;

    if (uMsg == WM_CREATE) {
      create_ = true;
      wp = wPar;
      lp = lPar;
    }

    if (callbacks) {
      if (create_) {
        if (callbacks->count(WM_CREATE)) {
          for (auto it : (*callbacks)[WM_CREATE]) {
            it(hWnd, wp, lp);
          }
          create_ = false;
        }
      }
      if (callbacks->count(uMsg)) {
        for (auto it : (*callbacks)[uMsg]) {
          it(hWnd, wPar, lPar);
        }
      }
    }
    else {
      if (uMsg == WM_NULL && lPar == 0x1233) {
        callbacks = reinterpret_cast<std::map<UINT, std::vector<std::function<void(HWND, WPARAM&, LPARAM&)>>>*>(wPar);
      }
    }
    
    return DefWindowProcW(hWnd, uMsg, wPar, lPar);
  };

  window_class_.cbSize = sizeof(WNDCLASSEXA);
  window_class_.style = class_style;
  window_class_.lpfnWndProc = procedure ? procedure : window_proc;
  window_class_.cbClsExtra = 0;
  window_class_.cbWndExtra = 0;
  window_class_.hInstance = instance_handle;
  window_class_.hIcon = icon_handle;
  window_class_.hCursor = cursor_handle;
  window_class_.hbrBackground = background_color;
  window_class_.lpszMenuName = MAKEINTRESOURCEW(menu_id_);
  window_class_.lpszClassName = window_class_name_.c_str();
  window_class_.hIconSm = mini_icon_handle;
}

/// @fn ~WindowModule
/// @brief デフォルトデストラクタ
WindowModule::~WindowModule() {
}

/// @Initialize
/// @brief 初期化
/// @return 成功：true
bool WindowModule::Initialize() {
  if (window_handle_) return false;

  ImmDisableIME(-1); // このスレッドで禁止

  WORD ret = RegisterClassExW(&window_class_);

  _ASSERT_EXPR(ret, L"snlib WindowModule Initialize error RegistClassEx is failed");

  RECT client_rect = { 0, 0, client_width_, client_height_ };
  AdjustWindowRect(&client_rect,window_style_,false);
  non_client_width_ = client_rect.right - client_rect.left - client_width_;
  non_client_height_ = client_rect.bottom - client_rect.top - client_height_;

  window_handle_ = CreateWindowExW(
    WS_EX_APPWINDOW,
    window_class_name_.c_str(),
    caption_.c_str(),
    window_style_,
    position_x_,
    position_y_,
    client_width_ + non_client_width_,
    client_height_ + non_client_height_,
    nullptr,
    menu_handle_,
    instance_handle_,
    nullptr);

  ShowWindow(window_handle_, SW_SHOW);
  SetForegroundWindow(window_handle_);
  SetFocus(window_handle_);
  UpdateWindow(window_handle_);

  SendMessageW(window_handle_, WM_NULL, reinterpret_cast<WPARAM>(&callbacks_), 0x1233);

  return window_handle_ ? ret ? true : false : false;
}

/// @Finalize
/// @brief 終了
void WindowModule::Finalize() {
  if (!window_handle_) return;

  DestroyWindow(window_handle_);
  UnregisterClassW(window_class_name_.c_str(), instance_handle_);
  window_handle_ = nullptr;
}

/// @fn MessageProcessing
/// @brief メッセージ処理
/// @return メッセージを処理した：true 終了メッセージが来た：false
bool WindowModule::MessageProcessing() {
  MSG message;
  for (;;) {
    if (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        return false;
      }
      else {
        TranslateMessage(&message);
        DispatchMessageW(&message);
      }
    }
    else {
      return true;
    }
  }
  return false;
}

/// @fn SetMessageCallback
/// @brief メッセージコールバック設定
/// @param[in] msg_type メッセージタイプ
/// @param[in] func コールバック関数
void WindowModule::SetMessageCallback(
  UINT msg_type,
  std::function<void(HWND, WPARAM&, LPARAM&)> func) {
  callbacks_[msg_type].push_back(func);
}


//EOF