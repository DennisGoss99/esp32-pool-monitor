#pragma once
#include <stdint.h>

template <typename T, uint8_t SIZE>
class MovingAverage {
public:
  MovingAverage() : _index(0), _count(0), _sum(0) {}

  void add(T value) {
    if (_count == SIZE) {
      _sum -= _values[_index];
    } else {
      _count++;
    }
    _values[_index] = value;
    _sum += value;
    _index = (_index + 1) % SIZE;
  }

  T get() const {
    if (_count == 0) return 0;
    return _sum / _count;
  }

private:
  T       _values[SIZE];
  uint8_t _index;
  uint8_t _count;
  T       _sum;
};
