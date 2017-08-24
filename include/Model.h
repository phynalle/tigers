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
  explicit Model();

  // Move and copy constructors are not allowed because binding attributes is
  // better be written once.
  Model(const Model&) = delete;
  Model(Model&&) = delete;

  // Assign operators must not copy or move attrs_ because it has references
  // to variables in model objects allocated.
  Model& operator=(const Model &);
  Model& operator=(Model &&);
  virtual ~Model() = default;

  bool HasAttribute(const std::string& name) const;
  bool HasRowKey() const;
  const RowKey& row_key() const;

 protected:
  bool bindAttr(const std::string& attr_name, AttributeBinder binder);
  AttributePair attributes() const;

 private:
  void set_ops_context(ops_context_t ctx);
  void set_row_key(const RowKey& key);

  ops_context_t ctx_;
  std::string row_key_;
  std::unordered_map<std::string, AttributeBinder> attrs_;

 public:
  class Accessor {
   protected: 
    void set_ops_context(Model& model, ops_context_t ctx) {
      model.set_ops_context(ctx);
    }
    
    void set_row_key(Model& model, const RowKey& key) {
      model.set_row_key(key);
    }

    AttributePair attributes(const Model& model) const {
      return model.attributes();
    }
  };

  friend class Accessor;
};

} /* namespace tigers */

#endif /* TIGERS_MODEL_H_ */