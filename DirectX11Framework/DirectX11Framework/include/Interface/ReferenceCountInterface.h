/// @file ReferenceCountInterface.h
/// @brief 参照カウンター機能の規定クラス
/// @date 2015/10/02
/// @author syuli nishida

#include <string.h>

/// @namespace snlib
namespace snlib {

class ReferenceCountInterface {
public:
  /// @fn AddRef
  /// @brief 参照カウンタ加算
  inline unsigned long AddRef() { return ++ _counter; }

  /// @fn Release
  /// @brief 参照カウンタ減算 0で消滅
  inline unsigned long Release() {
    if (_counter == 1) {
      delete this;
      return 0;
    }
    return -- _counter;
  }

  /// @fn GetIdentifier
  /// @brief 識別子取得
  inline const char* GetIdentifier() { return _identifier; }

protected:
  /// @fn ReferenceCountInterface
  /// @brief コンストラクタ
  /// @param[in] identifier クラスを一意にする識別子 15文字まで
  inline ReferenceCountInterface(const char* identifier) {
    memcpy_s(_identifier, 16, identifier, strlen(identifier));
  }

private:
  unsigned long _counter;
  char _identifier[16];
};

} // namespace snlib

//EOF