/// @file ReferenceCountInterface.h
/// @brief �Q�ƃJ�E���^�[�@�\�̋K��N���X
/// @date 2015/10/02
/// @author syuli nishida
#pragma once

#include <string.h>

#define SNLIB_DEFAULT_RELEASE_FUNCTION inline unsigned long Release(){if(_counter==1){delete this;return 0;}return --_counter;}

/// @namespace snlib
namespace snlib {

class ReferenceCountInterface {
public:
  /// @fn AddRef
  /// @brief �Q�ƃJ�E���^���Z
  inline unsigned long AddRef() { return ++ _counter; }

  /// @fn Release
  /// @brief �Q�ƃJ�E���^���Z 0�ŏ���
  virtual unsigned long Release() = 0;

  /// @fn GetIdentifier
  /// @brief ���ʎq�擾
  inline const char* GetIdentifier() { return _identifier; }

protected:
  /// @fn ReferenceCountInterface
  /// @brief �R���X�g���N�^
  /// @param[in] identifier �N���X����ӂɂ��鎯�ʎq 15�����܂�
  inline ReferenceCountInterface(const char* identifier) : _counter(1) {
    memcpy_s(_identifier, 16, identifier, strlen(identifier));
  }

  unsigned long _counter;

private:
  char _identifier[16];
};

} // namespace snlib

//EOF