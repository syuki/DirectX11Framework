/// @file DataContainer.cpp
/// @brief データコンテナ
/// @date 2015/10/09
/// @author syuki nishida

#include "Util/DataContainer.h"

using namespace snlib;

std::map<std::string, RawData> DataContainer::_data_container;

/// @fn Regist
/// @brief コンテナに登録
/// @param[in] filename ファイル名
/// @return 登録済み false
bool DataContainer::Regist(const char* filename) {
  if (_data_container.count(filename)) {
    return false;
  }
  bool success = _data_container[filename].LoadFromFile(filename);
  _ASSERT_EXPR(success, L"DataContainer : ファイルが読めませんでした");
  return true;
}

/// @fn Unregist
/// @brief 登録解除
/// @return 登録なし false
bool DataContainer::Unregist(const char* filename) {
  if (!_data_container.count(filename)) {
    return false;
  }
  _data_container.erase(filename);
  return true;
}

/// @fn Get
/// @brief データ取得 未登録の場合は登録しにいく
RawData* DataContainer::Get(const char* filename) {
  if (!_data_container.count(filename)) {
    Regist(filename);
  }
  return &_data_container[filename];
}

//EOF