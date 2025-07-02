#pragma once

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_multiset_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/range/iterator_range.hpp>
#include <cstdint>
#include <optional>

template <typename T>
class VNTable {
public:
  using ValueNumber = uint64_t;
  using EntityType = boost::bimaps::unordered_set_of<T>;
  using ValueNumberType = boost::bimaps::unordered_multiset_of<ValueNumber>;
  using Table = boost::bimap<EntityType, ValueNumberType>;

  using OptionalValueNumber = std::optional<ValueNumber>;

  OptionalValueNumber value(T aKey) const {
    auto it = fTable.left.find(aKey);
    if (it == fTable.left.end()) {
      return {};
    }
    return it->second;
  }

  using CongruenceIterator = typename Table::right_const_iterator;
  using Congruence = std::pair<CongruenceIterator, CongruenceIterator>;

  Congruence congruence(ValueNumber aValue) const {
    return fTable.right.equal_range(aValue);
  }

  void insertOrReplace(T aKey, ValueNumber aValue) {
    auto it = fTable.left.find(aKey);
    if (it == fTable.left.end()) {
      fTable.left.insert({aKey, aValue});
    } else {
      fTable.left.replace_data(it, aValue);
    }
  }

  void clear() {
    fTable.clear();
  }

private:
  Table fTable;
};
