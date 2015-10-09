/// @file VertexShader.h
/// @brief ���_�V�F�[�_
/// @date 2015/10/09
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include "Util/RawData.h"
#include "Util/SafeFunctions.h"

/// @namespace snlib
namespace snlib {

/// @class VertexShader
class VertexShader {
public:
  /// @fn VertexShader
  /// @brief �R���X�g���N�^
  VertexShader(ID3D11Device* device, RawData* data) {
    HRESULT ret = device->CreateVertexShader(data->GetPointer(), data->GetSize(), nullptr, &_vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(ret), L"VertexShader : �f�[�^�Ɉُ킪����܂�");
  }

  /// @fn ~VertexShader
  /// @brief �f�X�g���N�^
  virtual ~VertexShader() {
    SafeRelease(_vertex_shader);
  }

  /// @fn SetVertexShader
  /// @brief ���_�V�F�[�_�ݒ�
  /// @param[in] context �f�o�C�X�R���e�L�X�g
  void SetVertexShader(ID3D11DeviceContext* context) {
    context->VSSetShader(_vertex_shader, nullptr, 0);
  }

protected:
private:
  ID3D11VertexShader* _vertex_shader;
};

} // namespace snlib

//EOF