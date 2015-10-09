/// @file VertexBuffer.h
/// @brief 頂点バッファ
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
  /// @brief コンストラクタ
  VertexBuffer(ID3D11Device* device, void* data, UINT size, UINT stride) {

    // 頂点バッファ設定
    D3D11_BUFFER_DESC buffer_desc = { 0 };
    buffer_desc.ByteWidth = size;
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;

    // サブリソースの設定
    D3D11_SUBRESOURCE_DATA subresource = { 0 };
    subresource.pSysMem = data;
    subresource.SysMemPitch = 0;
    subresource.SysMemSlicePitch = 0;

    // 頂点バッファ作成
    HRESULT ret = device->CreateBuffer(&buffer_desc, &subresource, &_vertex_buffer);
    _ASSERT_EXPR(SUCCEEDED(ret), L"VertexBuffer : 頂点バッファ作成失敗");

    _stride = stride;
  }

  /// @fn ~VertexBuffer
  /// @brief デストラクタ
  virtual ~VertexBuffer() {
    SafeRelease(_vertex_buffer);
  }

  /// @fn SetVertexBuffer
  /// @brief 頂点バッファ設定
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