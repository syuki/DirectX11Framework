/// @file DataContainer.h
/// @brief ƒf[ƒ^ƒRƒ“ƒeƒi
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
  /// @brief ƒRƒ“ƒeƒi‚É“o˜^
  /// @param[in] filename ƒtƒ@ƒCƒ‹–¼
  /// @return “o˜^Ï‚İ false
  static bool Regist(const char* filename);

  /// @fn Unregist
  /// @brief “o˜^‰ğœ
  /// @return “o˜^‚È‚µ false
  static bool Unregist(const char* filename);

  /// @fn Get
  /// @brief ƒf[ƒ^æ“¾ –¢“o˜^‚Ìê‡‚Í“o˜^‚µ‚É‚¢‚­
  static RawData* Get(const char* filename);

protected:
private:
  static std::map<std::string, RawData> _data_container;
};

} // namespace snlib

//EOF