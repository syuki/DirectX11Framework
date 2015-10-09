/// @file VertexBuffer.h
/// @brief ���_�o�b�t�@
/// @date 2015/10/09
/// @author syuki nishida
#pragma once

#include <d3d11.h>
#include <crtdbg.h>
#include "Util/SafeFunctions.h"

/// @namespace snlib
namespace snlib {

/// @class VertexBuffer
class VertexBuffer {
public:
  /// @fn VertexBuffer
  /// @brief �R���X�g���N�^
  VertexBuffer(ID3D11Device* device, void* data, UINT size, UINT stride) {

    // ���_�o�b�t�@�ݒ�
    D3D11_BUFFER_DESC buffer_desc = { 0 };
    buffer_desc.ByteWidth = size;
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;

    // �T�u���\�[�X�̐ݒ�
    D3D11_SUBRESOURCE_DATA subresource = { 0 };
    subresource.pSysMem = data;
    subresource.SysMemPitch = 0;
    subresource.SysMemSlicePitch = 0;

    // ���_�o�b�t�@�쐬
    HRESULT ret = device->CreateBuffer(&buffer_desc, &subresource, &_vertex_buffer);
    _ASSERT_EXPR(SUCCEEDED(ret), L"VertexBuffer : ���_�o�b�t�@�쐬���s");

    _stride = stride;
  }

  /// @fn ~VertexBuffer
  /// @brief �f�X�g���N�^
  virtual ~VertexBuffer() {
    SafeRelease(_vertex_buffer);
  }

  /// @fn SetVertexBuffer
  /// @brief ���_�o�b�t�@�ݒ�
  void SetVertexBuffer(ID3D11DeviceContext* context, UINT slot) {
    UINT offset = 0;
    context->IASetVertexBuffers(slot, 1, &_vertex_buffer, &_stride, &offset);
  }

protected:
private:
  ID3D11Buffer* _vertex_buffer;
  UINT _stride;
};

} // namespace snlib

//EOF