#pragma once
#include <stdint.h>

template <typename T, uint8_t MAX_SIZE>
class MovingAverage {
public:
  MovingAverage() : _size(MAX_SIZE), _index(0), _count(0), _sum(0) {}

  void setSize(uint8_t size) {
    _size  = (size < 1) ? 1 : (size > MAX_SIZE ? MAX_SIZE : size);
    _index = 0;
    _count = 0;
    _sum   = 0;
  }

  void add(T value) {
    if (_count == _size) {
      _sum -= _values[_index];
    } else {
      _count++;
    }
    _values[_index] = value;
    _sum   += value;
    _index  = (_index + 1) % _size;
  }

  T get() const {
    if (_count == 0) return 0;
    return _sum / _count;
  }

private:
  T       _values[MAX_SIZE];
  uint8_t _size;
  uint8_t _index;
  uint8_t _count;
  T       _sum;
};
