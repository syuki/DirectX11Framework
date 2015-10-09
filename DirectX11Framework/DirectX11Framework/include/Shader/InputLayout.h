/// @file InputLayout.h
/// @brief インプットレイアウト
/// @date 2015/10/09
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include "Util/RawData.h"
#include "Util/SafeFunctions.h"

/// @namespace snlib
namespace snlib {

/// @class InputLayout
class InputLayout {
public:
  /// @fn InputLayout
  /// @brief コンストラクタ
  InputLayout(ID3D11Device* device, RawData* data, D3D11_INPUT_ELEMENT_DESC* elem_desc, UINT elem_count) {
    HRESULT ret = device->CreateInputLayout(elem_desc, elem_count, data->GetPointer(), data->GetSize(), &_input_layout);
    _ASSERT_EXPR(SUCCEEDED(ret), L"InputLayout : データに異常があります");
  }

  /// @fn ~InputLayout
  /// @brief デストラクタ
  virtual ~InputLayout() {
    _input_layout->Release();
  }

  /// @fn SetInputLayput
  /// @brief インプットレイアウト設定
  /// @param[in] context デバイスコンテキスト
  void SetInputLayout(ID3D11DeviceContext* context) {
    context->IASetInputLayout(_input_layout);
  }

protected:
private:
  ID3D11InputLayout* _input_layout;
};

} // namespace snlib

//EOF