/// @file DataContainer.cpp
/// @brief ƒf[ƒ^ƒRƒ“ƒeƒi
/// @date 2015/10/09
/// @author syuki nishida

#include "Util/DataContainer.h"

using namespace snlib;

std::map<std::string, RawData> DataContainer::_data_container;

/// @fn Regist
/// @brief ƒRƒ“ƒeƒi‚É“o˜^
/// @param[in] filename ƒtƒ@ƒCƒ‹–¼
/// @return “o˜^Ï‚İ false
bool DataContainer::Regist(const char* filename) {
  if (_data_container.count(filename)) {
    return false;
  }
  bool success = _data_container[filename].LoadFromFile(filename);
  _ASSERT_EXPR(success, L"DataContainer : ƒtƒ@ƒCƒ‹‚ª“Ç‚ß‚Ü‚¹‚ñ‚Å‚µ‚½");
  return true;
}

/// @fn Unregist
/// @brief “o˜^‰ğœ
/// @return “o˜^‚È‚µ false
bool DataContainer::Unregist(const char* filename) {
  if (!_data_container.count(filename)) {
    return false;
  }
  _data_container.erase(filename);
  return true;
}

/// @fn Get
/// @brief ƒf[ƒ^æ“¾ –¢“o˜^‚Ìê‡‚Í“o˜^‚µ‚É‚¢‚­
RawData* DataContainer::Get(const char* filename) {
  if (!_data_container.count(filename)) {
    Regist(filename);
  }
  return &_data_container[filename];
}

//EOF