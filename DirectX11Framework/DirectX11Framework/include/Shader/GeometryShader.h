/// @file GeometryShader.h
/// @brief ジオメトリシェーダ
/// @date 2015/10/09
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include "Util/RawData.h"
#include "Util/SafeFunctions.h"

/// @namespace snlib
namespace snlib {

/// @class GeometryShader
class GeometryShader {
public:
  /// @fn GeometryShader
  /// @brief コンストラクタ
  GeometryShader(ID3D11Device* device, RawData* data) {
    HRESULT ret = device->CreateGeometryShader(data->GetPointer(), data->GetSize(), nullptr, &_geometry_shader);
    _ASSERT_EXPR(SUCCEEDED(ret), L"GeometryShader : データに異常があります");
  }

  /// @fn ~GeometryShader
  /// @brief デストラクタ
  virtual ~GeometryShader() {
    SafeRelease(_geometry_shader);
  }

  /// @fn SetGeometryShader
  /// @brief ジオメトリシェーダ設定
  /// @param[in] context デバイスコンテキスト
  void SetGeometryShader(ID3D11DeviceContext* context) {
    context->GSSetShader(_geometry_shader, nullptr, 0);
  }

protected:
private:
  ID3D11GeometryShader* _geometry_shader;
};

} // namespace snlib

//EOF