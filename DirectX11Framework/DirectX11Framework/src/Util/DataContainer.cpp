/// @file DataContainer.cpp
/// @brief �f�[�^�R���e�i
/// @date 2015/10/09
/// @author syuki nishida

#include "Util/DataContainer.h"

using namespace snlib;

std::map<std::string, RawData> DataContainer::_data_container;

/// @fn Regist
/// @brief �R���e�i�ɓo�^
/// @param[in] filename �t�@�C����
/// @return �o�^�ς� false
bool DataContainer::Regist(const char* filename) {
  if (_data_container.count(filename)) {
    return false;
  }
  bool success = _data_container[filename].LoadFromFile(filename);
  _ASSERT_EXPR(success, L"DataContainer : �t�@�C�����ǂ߂܂���ł���");
  return true;
}

/// @fn Unregist
/// @brief �o�^����
/// @return �o�^�Ȃ� false
bool DataContainer::Unregist(const char* filename) {
  if (!_data_container.count(filename)) {
    return false;
  }
  _data_container.erase(filename);
  return true;
}

/// @fn Get
/// @brief �f�[�^�擾 ���o�^�̏ꍇ�͓o�^���ɂ���
RawData* DataContainer::Get(const char* filename) {
  if (!_data_container.count(filename)) {
    Regist(filename);
  }
  return &_data_container[filename];
}

//EOF