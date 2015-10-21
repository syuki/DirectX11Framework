/// @file FrameKeeper.h
/// @brief FPS�����
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
  /// �R���X�g���N�^
  template<class _Fn, class... _Args>
  FrameKeeper(const UINT& target_frame, _Fn function, _Args... args) :
    target_frame_(target_frame),
    fps_counter_(0),
    respond_fps_(0) {
    // �֐����
    function_ = [function, args...] {
      function(args...);
    };

    // ���g���擾
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    frequency_ = freq.QuadPart;

    // ������
    LARGE_INTEGER cur_cnt;
    QueryPerformanceCounter(&cur_cnt);
    fps_last_time_ = update_last_time_ = cur_cnt.QuadPart;
  }

  /// �f�X�g���N�^
  virtual ~FrameKeeper() {}

  /// �X�e�b�v
  bool Step() {
    // ���̎���
    LARGE_INTEGER buff;
    QueryPerformanceCounter(&buff);
    long long cur_time = buff.QuadPart;

    // FPS�v���i500�t���[���i0.5�b�j�j
    if (((cur_time - fps_last_time_) * 1000 / frequency_) >= (500)){

      // FPS �v�Z
      respond_fps_ = fps_counter_ * 1000 / ((cur_time - fps_last_time_) * 1000 / frequency_);

      // �ŏI���ԍX�V
      fps_last_time_ = cur_time;

      // �J�E���g������
      fps_counter_ = 0;
    }

    // FPS
    if (((cur_time - update_last_time_) * 1000 / frequency_) >= (1000 / target_frame_)) {

      // �ŏI�X�V�X�V
      update_last_time_ = cur_time;

      // �t���[�����Z
      fps_counter_++;

      // �֐��R�[��
      function_();
      return true;
    }

    return false;
  }

  /// FPS�擾
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