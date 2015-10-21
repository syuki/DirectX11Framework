/// @file Context.h
/// @brief �R���e�L�X�g
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
  /// @brief �R���e�L�X�g�쐬
  /// @return �R���e�L�X�g
  inline static Context* CreateContext(ID3D11DeviceContext* context) {
    return new Context(context);
  }

  /// @fn GetContext
  /// @brief Dx11�̃R���e�L�X�g�擾
  inline ID3D11DeviceContext* GetContext() {
    return _context;
  }

  SNLIB_DEFAULT_RELEASE_FUNCTION
protected:
private:
  /// @fn Context
  /// @brief �R���X�g���N�^
  inline Context(ID3D11DeviceContext* context) :
    ReferenceCountInterface("dx11context"),
    _context(context) {
    _context->AddRef();
  }

  /// @fn ~Context
  /// @brief �f�X�g���N�^
  inline virtual ~Context() {
    _context->Release();
  }

  ID3D11DeviceContext* _context;
};

} // namespace snlib

//EOF