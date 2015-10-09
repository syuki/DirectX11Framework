/// @file raw_data.h
/// @brief 生のデータ
/// @date 2015/04/12
/// @suthor syuki nihsida
#pragma once

#include <crtdbg.h>
#include <windows.h>
#include <stdio.h>
#include <errno.h>

/// @namespace snlib
namespace snlib {
/// @class RawFile
class RawData {
public:

  /// @fn RawData
  /// @brief デフォルトコンストラクタ
  inline RawData() : pointer_(nullptr), size_(0) {}

  /// @fn RawData
  /// @brief ムーブコンストラクタ
  /// @param[in] src 移動元
  inline RawData(RawData&& src)
    : pointer_(src.pointer_),
    size_(src.size_) {
    src.pointer_ = nullptr;
    src.size_ = 0;
  }

  /// @fn RawData
  /// @brief ファイルを指定して読み込む
  /// @param[in] filename ファイル名
  inline RawData(const char* filename) {
    LoadFromFile(filename);
  }

  /// @fn RawData
  /// @brief ポインタとサイズを渡して読み込む
  /// @param[in] pointer 読み込ませたい
  /// @param[in] size 渡したいサイズ
  inline RawData(void* pointer, size_t size) {
    LoadFromPointer(pointer_, size);
  }

  /// @fn ~RawData
  /// @brief デストラクタ
  inline virtual ~RawData() {
    Free();
  }

  /// @fn Allocate
  /// @brief メモリ確保
  /// @param[in] size 確保サイズ
  /// @param[in] force 強行するか
  /// @return 確保できたらtrue
  inline bool Allocate(size_t size, bool force) {
    _ASSERT_EXPR(size, L"\"RawData\" : allocate size is zero.");
    if (size_ && !force) return false;
    Free();

    size_ = size;
    pointer_ = new char[size_];

    return true;
  }

  /// @fn Free
  /// @brief 確保したメモリ解放
  /// @return 解放できたらtrue
  inline bool Free() {
    if (!size_) return false;

    delete[] pointer_;
    pointer_ = nullptr;
    size_ = 0;

    return true;
  }

  /// @fn Copy
  /// @brief 引数のメモリをコピーする
  ///        メモリが確保してある場合はそのまま使う
  ///        足らない場合はこの関数は失敗する
  /// @param[in] source コピーソース
  /// @param[in] size サイズ
  /// @param[in] clear メモリ内をクリアするか
  /// @return コピーできたらture
  inline bool Copy(void* source, size_t size, bool clear = true) {
    if (size > size_) return false;

    if (clear) {
      ZeroMemory(pointer_, size_);
    }
    memcpy_s(pointer_, size_, source, size);

    return true;
  }

  /// @fn LoadFromFile
  /// @brief ファイルから読み込み
  /// @param[in] filename ファイル名
  /// @return 読み込めたらtrue
  inline bool LoadFromFile(const char* filename) {
    FILE* fp;
    int ret = fopen_s(&fp, filename, "rb");
    if (ret == EINVAL) return false;

    fpos_t file_size;
    fseek(fp, 0, SEEK_END);
    fgetpos(fp, &file_size);
    fseek(fp, 0, SEEK_SET);
    if (file_size > UINT_MAX || !file_size) {
      _ASSERT_EXPR(file_size <= UINT_MAX, L"\"RawData\" : file size is big.");
      _ASSERT_EXPR(file_size, L"\"RawData\" : file size is zero.");
      fclose(fp);
      return false;
    }

    size_ = static_cast<size_t>(file_size);
    char* buffer = new char[size_];
    fread_s(buffer, size_, sizeof(char), size_, fp);

    Allocate(size_, true);
    Copy(buffer, size_);

    return true;
  }

  /// @fn LoadFromPointer
  /// @brief ポインタから読み込む
  ///        メモリが確保されている場合は破棄して取り直す
  /// @param[in] pointer 読みたいポインタ
  /// @param[in] size 読ませたいサイズ
  /// @param[in] force 強行するか
  /// @return 読み込めたらtrue
  inline bool RawData::LoadFromPointer(void* pointer, size_t size, bool force = false) {
    if (!Allocate(size, true)) return false;
    return Copy(pointer, size);
  }

  /// @fn GetPointer
  /// @brief ポインタ取得
  /// @return ポインタ
  __forceinline void* GetPointer() { return pointer_; }

  /// @fn GetSize
  /// @brief サイズ取得
  /// @return サイズ
  __forceinline size_t GetSize() { return size_; }

  /// @fn Empty
  /// @brief 値が入っていないか
  /// @return 空ならtrue
  __forceinline bool Empty() { return pointer_ ? false : true; }

protected:

private:

  void* pointer_;
  size_t size_;
};

} // namespace snlib

//EOF