/// @file SafeFunctions.h
/// @brief nullチェックする関数とか
/// @date 2015/10/02
/// @author syuki nishida
#pragma once

#include <crtdbg.h>

/// @namespace snlib
namespace snlib {

/// @fn SafeDelete
template<class T>inline void SafeDelete(T a) {
  if (!a) return;
  delete a;
  a = nullptr;
}

/// @fn SafeDeleteArray
template<class T>inline void SafeDeleteArray(T a) {
  if (!a) return;
  delete[] a;
  a = nullptr;
}

/// @fn SafeRelease
template<class T>inline void SafeRelease(T a) {
  if (!a) return;
  unsigned long count = a->Release();
  _ASSERT_EXPR(!count, L"Release miss");
  a = nullptr;
}

} // namespace snlib

//EOF