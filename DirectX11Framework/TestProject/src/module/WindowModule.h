/// @file WindowModule.h
/// @brief �E�B���h�E���W���[��
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
  /// @biref �f�t�H���g�R���X�g���N�^
  WindowModule();

  /// @fn WindowModule
  /// @brief �L���v�V�����ƃN���C�A���g�T�C�Y
  /// @param[in] caption �L���v�V����
  /// @param[in] client_width �N���C�A���g��
  /// @param[in] client_height �N���C�A���g����
  WindowModule(LPCWSTR caption, int client_width, int client_height);

  /// @fn WindowModule
  /// @brief �L���v�V�����ƃN���C���g�T�C�Y�ƃC���X�^���X�n���h��
  /// @param[in] caption �L���v�V����
  /// @param[in] client_width �N���C�A���g��
  /// @param[in] client_hright �N���C���g����
  /// @param[in] instance_handle �C���X�^���X�n���h��
  WindowModule(LPCWSTR caption, int client_width, int client_height,
               HINSTANCE instance_handle);

  /// @fn WindowModule
  /// @brief �S�������R���X�g���N�^
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
  /// @brief �f�t�H���g�f�X�g���N�^
  virtual ~WindowModule();

  /// @Initialize
  /// @brief ������
  /// @return �����Ftrue
  bool Initialize();

  /// @Finalize
  /// @brief �I��
  void Finalize();

  /// @fn MessageProcessing
  /// @brief ���b�Z�[�W����
  /// @return ���b�Z�[�W�����������Ftrue �I�����b�Z�[�W�������Ffalse
  bool MessageProcessing();

  /// @fn GetWindowHandle
  /// @brief �E�B���h�E�n���h���擾
  __forceinline const HWND GetWindowHandle() {
    return window_handle_;
  }

  /// @fn SetMessageCallback
  /// @brief ���b�Z�[�W�R�[���o�b�N�ݒ�
  /// @param[in] msg_type ���b�Z�[�W�^�C�v
  /// @param[in] func �R�[���o�b�N�֐�
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