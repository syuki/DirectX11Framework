/// @file PixelShader.h
/// @brief �s�N�Z���V�F�[�_
/// @date 2015/10/09
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include "Util/RawData.h"
#include "Util/SafeFunctions.h"

/// @namespace snlib
namespace snlib {

/// @class PixelShader
class PixelShader {
public:
  /// @fn PixelShader
  /// @brief �R���X�g���N�^
  PixelShader(ID3D11Device* device, RawData* data) {
    HRESULT ret = device->CreatePixelShader(data->GetPointer(), data->GetSize(), nullptr, &_pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(ret), L"PixelShader : �f�[�^�Ɉُ킪����܂�");
  }

  /// @fn ~PixelShader
  /// @brief �f�X�g���N�^
  virtual ~PixelShader() {
    SafeRelease(_pixel_shader);
  }

  /// @fn SetPixelShader
  /// @brief �s�N�Z���V�F�[�_�ݒ�
  /// @param[in] context �f�o�C�X�R���e�L�X�g
  void SetPixelShader(ID3D11DeviceContext* context) {
    context->PSSetShader(_pixel_shader, nullptr, 0);
  }

protected:
private:
  ID3D11PixelShader* _pixel_shader;
};

} // namespace snlib

//EOF