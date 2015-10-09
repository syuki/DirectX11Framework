/// @file DataContainer.h
/// @brief データコンテナ
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
  /// @brief コンテナに登録
  /// @param[in] filename ファイル名
  /// @return 登録済み false
  static bool Regist(const char* filename);

  /// @fn Unregist
  /// @brief 登録解除
  /// @return 登録なし false
  static bool Unregist(const char* filename);

  /// @fn Get
  /// @brief データ取得 未登録の場合は登録しにいく
  static RawData* Get(const char* filename);

protected:
private:
  /// @fn DataContainer
  /// @brief コンストラクタ
  DataContainer(){}

  /// @fn ~DataContainer
  /// @brief デストラクタ
  ~DataContainer(){}

  static std::map<std::string, RawData> _data_container;
};

} // namespace snlib

//EOF