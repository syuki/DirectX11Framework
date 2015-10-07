/// @file ReferenceCountInterface.h
/// @brief �Q�ƃJ�E���^�[�@�\�̋K��N���X
/// @date 2015/10/02
/// @author syuli nishida

#include <string.h>

/// @namespace snlib
namespace snlib {

class ReferenceCountInterface {
public:
  /// @fn AddRef
  /// @brief �Q�ƃJ�E���^���Z
  inline unsigned long AddRef() { return ++ _counter; }

  /// @fn Release
  /// @brief �Q�ƃJ�E���^���Z 0�ŏ���
  inline unsigned long Release() {
    if (_counter == 1) {
      delete this;
      return 0;
    }
    return -- _counter;
  }

  /// @fn GetIdentifier
  /// @brief ���ʎq�擾
  inline const char* GetIdentifier() { return _identifier; }

protected:
  /// @fn ReferenceCountInterface
  /// @brief �R���X�g���N�^
  /// @param[in] identifier �N���X����ӂɂ��鎯�ʎq 15�����܂�
  inline ReferenceCountInterface(const char* identifier) {
    memcpy_s(_identifier, 16, identifier, strlen(identifier));
  }

private:
  unsigned long _counter;
  char _identifier[16];
};

} // namespace snlib

//EOF