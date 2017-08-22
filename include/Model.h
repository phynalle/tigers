#ifndef TIGERS_MODEL_H_
#define TIGERS_MODEL_H_

#include <unordered_map>
#include <functional>
#include <vector>

#include "types.h" 

#include <iostream>

namespace tigers {

std::string CallGet(const AttributeBinder& binder);
std::string CallGet(std::reference_wrapper<const AttributeBinder> binder);
void CallSet(const AttributeBinder& binder, const std::string& value);
void CallSet(
    std::reference_wrapper<const AttributeBinder>,
    const std::string& value);

AttributeGetter EmptyGetter();
AttributeSetter EmptySetter();
AttributeBinder MakeStringBinder(std::reference_wrapper<std::string> s);

class Model {
 public:
  Model();
  Model(const Model&) = delete;
  Model(Model&&) = delete;
  Model &operator=(Model &&) = delete;
  Model &operator=(const Model &) = delete;
  virtual ~Model() = default;

  bool HasAttribute(const std::string& name) const;

  const RowKey& row_key() const;
  void set_row_key(const RowKey& key);
  
 protected:
  bool bindAttr(const std::string& attr_name, AttributeBinder binder);

 private:
  AttributePair attributes() const;

  // std::string name_;
  std::string row_key_;
  std::unordered_map<std::string, AttributeBinder> attrs_;
};

} /* namespace tigers */

#endif /* TIGERS_MODEL_H_ */