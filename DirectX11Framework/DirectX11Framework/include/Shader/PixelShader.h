/// @file PixelShader.h
/// @brief ピクセルシェーダ
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
  /// @brief コンストラクタ
  PixelShader(ID3D11Device* device, RawData* data) {
    HRESULT ret = device->CreatePixelShader(data->GetPointer(), data->GetSize(), nullptr, &_pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(ret), L"PixelShader : データに異常があります");
  }

  /// @fn ~PixelShader
  /// @brief デストラクタ
  virtual ~PixelShader() {
    SafeRelease(_pixel_shader);
  }

  /// @fn SetPixelShader
  /// @brief ピクセルシェーダ設定
  /// @param[in] context デバイスコンテキスト
  void SetPixelShader(ID3D11DeviceContext* context) {
    context->PSSetShader(_pixel_shader, nullptr, 0);
  }

protected:
private:
  ID3D11PixelShader* _pixel_shader;
};

} // namespace snlib

//EOF