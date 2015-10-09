/// @file raw_data.h
/// @brief ���̃f�[�^
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
  /// @brief �f�t�H���g�R���X�g���N�^
  inline RawData() : pointer_(nullptr), size_(0) {}

  /// @fn RawData
  /// @brief ���[�u�R���X�g���N�^
  /// @param[in] src �ړ���
  inline RawData(RawData&& src)
    : pointer_(src.pointer_),
    size_(src.size_) {
    src.pointer_ = nullptr;
    src.size_ = 0;
  }

  /// @fn RawData
  /// @brief �t�@�C�����w�肵�ēǂݍ���
  /// @param[in] filename �t�@�C����
  inline RawData(const char* filename) {
    LoadFromFile(filename);
  }

  /// @fn RawData
  /// @brief �|�C���^�ƃT�C�Y��n���ēǂݍ���
  /// @param[in] pointer �ǂݍ��܂�����
  /// @param[in] size �n�������T�C�Y
  inline RawData(void* pointer, size_t size) {
    LoadFromPointer(pointer_, size);
  }

  /// @fn ~RawData
  /// @brief �f�X�g���N�^
  inline virtual ~RawData() {
    Free();
  }

  /// @fn Allocate
  /// @brief �������m��
  /// @param[in] size �m�ۃT�C�Y
  /// @param[in] force ���s���邩
  /// @return �m�ۂł�����true
  inline bool Allocate(size_t size, bool force) {
    _ASSERT_EXPR(size, L"\"RawData\" : allocate size is zero.");
    if (size_ && !force) return false;
    Free();

    size_ = size;
    pointer_ = new char[size_];

    return true;
  }

  /// @fn Free
  /// @brief �m�ۂ������������
  /// @return ����ł�����true
  inline bool Free() {
    if (!size_) return false;

    delete[] pointer_;
    pointer_ = nullptr;
    size_ = 0;

    return true;
  }

  /// @fn Copy
  /// @brief �����̃��������R�s�[����
  ///        ���������m�ۂ��Ă���ꍇ�͂��̂܂܎g��
  ///        ����Ȃ��ꍇ�͂��̊֐��͎��s����
  /// @param[in] source �R�s�[�\�[�X
  /// @param[in] size �T�C�Y
  /// @param[in] clear �����������N���A���邩
  /// @return �R�s�[�ł�����ture
  inline bool Copy(void* source, size_t size, bool clear = true) {
    if (size > size_) return false;

    if (clear) {
      ZeroMemory(pointer_, size_);
    }
    memcpy_s(pointer_, size_, source, size);

    return true;
  }

  /// @fn LoadFromFile
  /// @brief �t�@�C������ǂݍ���
  /// @param[in] filename �t�@�C����
  /// @return �ǂݍ��߂���true
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
  /// @brief �|�C���^����ǂݍ���
  ///        ���������m�ۂ���Ă���ꍇ�͔j�����Ď�蒼��
  /// @param[in] pointer �ǂ݂����|�C���^
  /// @param[in] size �ǂ܂������T�C�Y
  /// @param[in] force ���s���邩
  /// @return �ǂݍ��߂���true
  inline bool RawData::LoadFromPointer(void* pointer, size_t size, bool force = false) {
    if (!Allocate(size, true)) return false;
    return Copy(pointer, size);
  }

  /// @fn GetPointer
  /// @brief �|�C���^�擾
  /// @return �|�C���^
  __forceinline void* GetPointer() { return pointer_; }

  /// @fn GetSize
  /// @brief �T�C�Y�擾
  /// @return �T�C�Y
  __forceinline size_t GetSize() { return size_; }

  /// @fn Empty
  /// @brief �l�������Ă��Ȃ���
  /// @return ��Ȃ�true
  __forceinline bool Empty() { return pointer_ ? false : true; }

protected:

private:

  void* pointer_;
  size_t size_;
};

} // namespace snlib

//EOF