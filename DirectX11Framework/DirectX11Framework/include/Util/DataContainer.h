/// @file DataContainer.h
/// @brief �f�[�^�R���e�i
/// @date 2015/10/09
/// @author syuki nishida
#pragma once

#include <map>
#include <string>
#include "RawData.h"

/// @namespace snlib
namespace snlib {

/// @class DataContainer
class DataContainer {
public:
  /// @fn Regist
  /// @brief �R���e�i�ɓo�^
  /// @param[in] filename �t�@�C����
  /// @return �o�^�ς� false
  static bool Regist(const char* filename);

  /// @fn Unregist
  /// @brief �o�^����
  /// @return �o�^�Ȃ� false
  static bool Unregist(const char* filename);

  /// @fn Get
  /// @brief �f�[�^�擾 ���o�^�̏ꍇ�͓o�^���ɂ���
  static RawData* Get(const char* filename);

protected:
private:
  /// @fn DataContainer
  /// @brief �R���X�g���N�^
  DataContainer(){}

  /// @fn ~DataContainer
  /// @brief �f�X�g���N�^
  ~DataContainer(){}

  static std::map<std::string, RawData> _data_container;
};

} // namespace snlib

//EOF