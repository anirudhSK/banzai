#ifndef FIELD_CONTAINER_H_
#define FIELD_CONTAINER_H_

#include <cassert>

#include <map>

/// FieldContainer: a map from the field name as a string
/// to the field value as an integer.
/// Two FieldContainers, can be added (unioned) together,
/// so long as they don't have the same fields
///  in them, in which case it isn't clear which
/// value should take precedence.
/// FieldContainers can be used to represent both packets with named fields
/// and state with named fields as part of that state.
class FieldContainer {
 public:
  /// Typedef for string as field name
  typedef std::string FieldName;

  /// Write field
  void write(const FieldName & field_name, const uint32_t & val) { field_map_[field_name] = val; }

  /// Read field
  uint32_t read(const FieldName & field_name) const {
    assert(field_map_.find(field_name) != field_map_.end());
    return field_map_.at(field_name);
  }

  /// Generate random FieldContainer with the same fields
  /// as the current one, but with all fields init. to random values.
  auto generate_random_field_map() {
    FieldContainer field_container;
    for (const auto & field_name : field_map_) {
      field_container.field_map_[field_name.first] = static_cast<uint32_t>(rand());
    }
    return field_container;
  }

  /// Overload += operator to merge a FieldContainer into this
  /// as long as they have no fields in common
  FieldContainer & operator+=(const FieldContainer & fc) {
    // Check that none of fc's keys are in this and vice versa.
    for (const auto & key_pair : fc.field_map_) assert(this->field_map_.find(key_pair.first) == this->field_map_.end());
    for (const auto & key_pair : this->field_map_) assert(fc.field_map_.find(key_pair.first) == fc.field_map_.end());

    // Collapse the fc key set
    for (const auto & key_pair : fc.field_map_) write(key_pair.first, key_pair.second);

    return *this;
  }

  /// Print to stream
  friend std::ostream & operator<< (std::ostream & out, const FieldContainer & field_container) {
    for (const auto & key_pair : field_container.field_map_) out << key_pair.first << " : " << key_pair.second << "\n";

    return out;
  }

 private:
  /// Map from FieldName to field value.
  std::map<FieldName, uint32_t> field_map_ = {};
};

#endif  // FIELD_CONTAINER_H_