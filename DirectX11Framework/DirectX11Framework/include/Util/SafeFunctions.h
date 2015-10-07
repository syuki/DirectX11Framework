/// @file SafeFunctions.h
/// @brief nullチェックする関数とか
/// @date 2015/10/02
/// @author syuki nishida
#pragma once

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
  a->Release();
  a = nullptr;
}

} // namespace snlib

//EOF