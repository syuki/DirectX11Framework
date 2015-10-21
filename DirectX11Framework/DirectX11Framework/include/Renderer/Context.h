/// @file Context.h
/// @brief コンテキスト
/// @date 2015/10/11
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include "Interface/ReferenceCountInterface.h"

/// @namespace snlib
namespace snlib {

/// @class Context
class Context : public ReferenceCountInterface {
public:
  /// @fn CreateContext
  /// @brief コンテキスト作成
  /// @return コンテキスト
  inline static Context* CreateContext(ID3D11DeviceContext* context) {
    return new Context(context);
  }

  /// @fn GetContext
  /// @brief Dx11のコンテキスト取得
  inline ID3D11DeviceContext* GetContext() {
    return _context;
  }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn Context
  /// @brief コンストラクタ
  inline Context(ID3D11DeviceContext* context) :
    ReferenceCountInterface("dx11context"),
    _context(context) {
    _context->AddRef();
  }

  /// @fn ~Context
  /// @brief デストラクタ
  inline virtual ~Context() {
    _context->Release();
  }

  ID3D11DeviceContext* _context;
};

} // namespace snlib

//EOF