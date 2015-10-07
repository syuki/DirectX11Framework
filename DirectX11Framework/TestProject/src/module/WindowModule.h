/// @file WindowModule.h
/// @brief ウィンドウモジュール
/// @data 2015/04/16
/// @author syuki nishida

#pragma once
#ifndef SNLIB_WINDOW_MODULE_H_
#define SNLIB_WINDOW_MODULE_H_

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <functional>

#define SN_DEFAULT_CLASS_STYLE        (CS_CLASSDC)
#define SN_DEFAULT_BACKGROUND_COLOR   (reinterpret_cast<HBRUSH>(COLOR_WINDOW+1))
#define SN_DEFAULT_MOUSE_CURSOR       (LoadCursor(NULL, IDC_ARROW))
#define SN_DEFAULT_WINDDOW_CLASS_NAME (L"window_class")
#define SN_DEFAULT_INSTANCE_HANDLE    (GetModuleHandle(nullptr))

#define SN_DEFAULT_WINDOW_STYLE       (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX)
#define SN_DEFAULT_CLIENT_WIDTH       (800)
#define SN_DEFAULT_CLIENT_HEIGHT      (600)

/// @namespace snlib
namespace snlib {

/// @class WindowModule
class WindowModule {
public:
  /// @fn WindowModule
  /// @biref デフォルトコンストラクタ
  WindowModule();

  /// @fn WindowModule
  /// @brief キャプションとクライアントサイズ
  /// @param[in] caption キャプション
  /// @param[in] client_width クライアント幅
  /// @param[in] client_height クライアント高さ
  WindowModule(LPCWSTR caption, int client_width, int client_height);

  /// @fn WindowModule
  /// @brief キャプションとクライントサイズとインスタンスハンドル
  /// @param[in] caption キャプション
  /// @param[in] client_width クライアント幅
  /// @param[in] client_hright クライント高さ
  /// @param[in] instance_handle インスタンスハンドル
  WindowModule(LPCWSTR caption, int client_width, int client_height,
               HINSTANCE instance_handle);

  /// @fn WindowModule
  /// @brief 全引数つきコンストラクタ
  WindowModule(
    LPCWSTR caption,
    LPCWSTR class_name = SN_DEFAULT_WINDDOW_CLASS_NAME,
    UINT class_style = SN_DEFAULT_CLASS_STYLE,
    HINSTANCE instance_handle = SN_DEFAULT_INSTANCE_HANDLE,
    WNDPROC procedure = nullptr,
    HICON icon_handle = nullptr,
    HCURSOR cursor_handle = SN_DEFAULT_MOUSE_CURSOR,
    HBRUSH background_color = SN_DEFAULT_BACKGROUND_COLOR,
    UINT menu_id = 0,
    HICON mini_icon_handle = nullptr,
    DWORD window_style = SN_DEFAULT_WINDOW_STYLE,
    int position_x = CW_USEDEFAULT,
    int position_y = CW_USEDEFAULT,
    int client_width = SN_DEFAULT_CLIENT_WIDTH,
    int client_height = SN_DEFAULT_CLIENT_HEIGHT,
    HMENU menu_handle = nullptr
    );

  /// @fn ~WindowModule
  /// @brief デフォルトデストラクタ
  virtual ~WindowModule();

  /// @Initialize
  /// @brief 初期化
  /// @return 成功：true
  bool Initialize();

  /// @Finalize
  /// @brief 終了
  void Finalize();

  /// @fn MessageProcessing
  /// @brief メッセージ処理
  /// @return メッセージを処理した：true 終了メッセージが来た：false
  bool MessageProcessing();

  /// @fn GetWindowHandle
  /// @brief ウィンドウハンドル取得
  __forceinline const HWND GetWindowHandle() {
    return window_handle_;
  }

  /// @fn SetMessageCallback
  /// @brief メッセージコールバック設定
  /// @param[in] msg_type メッセージタイプ
  /// @param[in] func コールバック関数
  void SetMessageCallback(UINT msg_type, std::function<void(HWND,WPARAM&,LPARAM&)>);

private:
  WNDCLASSEXW   window_class_;

  std::wstring  window_class_name_;
  UINT          menu_id_;
  std::wstring  caption_;

  DWORD         window_style_;
  int           position_x_;
  int           position_y_;
  int           client_width_;
  int           client_height_;
  int           non_client_width_;
  int           non_client_height_;
  HMENU         menu_handle_;

  HINSTANCE instance_handle_;
  HWND        window_handle_;

  std::map<UINT,std::vector<std::function<void(HWND,WPARAM&,LPARAM&)>>> callbacks_;

protected:
};

} // namespace snlib

#endif // SNLIB_WINDOW_MODULE_H_

//EOF