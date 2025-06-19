#pragma once

#include <algorithm>
#include <memory>

template <typename T>
class BinarySearch {
public:
  BinarySearch(std::shared_ptr<T> aData) : fData(aData) {
    if (fData.expired()) {
      throw std::logic_error("Cannot bind to an expired container");
    }

    auto data = fData.lock();
    // Ensure the data is sorted
    std::ranges::sort(*data);
  }

  int rank(const typename T::value_type& aSearch);

private:
  std::weak_ptr<T> fData;
};

template <typename T>
int BinarySearch<T>::rank(const typename T::value_type& aSearch) {
  if (fData.expired()) {
    throw std::logic_error("Operation on unbound container");
  }
  auto data = fData.lock();
  int low = 0;
  int high = data->size() - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    if ((*data)[mid] < aSearch) {
      low = mid + 1;
    } else if ((*data)[mid] > aSearch) {
      high = mid - 1;
    } else {
      return mid; // Found
    }
  }

  return -1; // Not found
}
