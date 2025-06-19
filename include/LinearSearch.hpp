#pragma once

#include <memory>
#include <stdexcept>

template <typename T>
class LinearSearch {
public:
  LinearSearch(std::shared_ptr<T> aData) : fData(aData) {
  }
  int rank(const typename T::value_type& aSearch);

private:
  std::weak_ptr<T> fData;
};

template <typename T>
int LinearSearch<T>::rank(const typename T::value_type& aSearch) {
  if (fData.expired()) {
    throw std::logic_error("Operation on unbound container");
  }
  auto data = fData.lock();
  int index = -1;
  for (const auto& element : *data) {
    index++;
    if (element == aSearch) {
      return index;
    }
  }
  return -1; // Not found
}
