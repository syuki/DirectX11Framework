/// @file FrameKeeper.h
/// @brief FPSを守る
/// @date 2014/11/14
/// @author syuki nishida

#pragma once
#ifndef SNLIB_FREAME_KEEPER_H_
#define SNLIB_FREAME_KEEPER_H_

#include <Windows.h>
#include <functional>

/// @namespace snlib
namespace snlib {

/// @class FrameKeeper
class FrameKeeper {
public:
  /// コンストラクタ
  template<class _Fn, class... _Args>
  FrameKeeper(const UINT& target_frame, _Fn function, _Args... args) :
    target_frame_(target_frame),
    fps_counter_(0),
    respond_fps_(0) {
    // 関数代入
    function_ = [function, args...] {
      function(args...);
    };

    // 周波数取得
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    frequency_ = freq.QuadPart;

    // 初期化
    LARGE_INTEGER cur_cnt;
    QueryPerformanceCounter(&cur_cnt);
    fps_last_time_ = update_last_time_ = cur_cnt.QuadPart;
  }

  /// デストラクタ
  virtual ~FrameKeeper() {}

  /// ステップ
  bool Step() {
    // 今の時間
    LARGE_INTEGER buff;
    QueryPerformanceCounter(&buff);
    long long cur_time = buff.QuadPart;

    // FPS計測（500フレーム（0.5秒））
    if (((cur_time - fps_last_time_) * 1000 / frequency_) >= (500)){

      // FPS 計算
      respond_fps_ = fps_counter_ * 1000 / ((cur_time - fps_last_time_) * 1000 / frequency_);

      // 最終時間更新
      fps_last_time_ = cur_time;

      // カウント初期化
      fps_counter_ = 0;
    }

    // FPS
    if (((cur_time - update_last_time_) * 1000 / frequency_) >= (1000 / target_frame_)) {

      // 最終更新更新
      update_last_time_ = cur_time;

      // フレーム加算
      fps_counter_++;

      // 関数コール
      function_();
      return true;
    }

    return false;
  }

  /// FPS取得
  LONG64& GetFps() { return respond_fps_; }

protected:
private:
  UINT target_frame_;
  LONG64 fps_counter_;
  LONG64 respond_fps_;
  std::function<void()> function_;
  LONG64 frequency_;
  LONG64 fps_last_time_;
  LONG64 update_last_time_;
};

} // namespace snlib

#endif // SNLIB_FREAME_KEEPER_H_

//EOF