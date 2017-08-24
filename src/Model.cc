#include "Model.h"

tigers::Model::Model() 
    : ctx_({nullptr, nullptr}) {

}

tigers::Model& tigers::Model::operator=(const Model& other) {
  ctx_ = other.ctx_;
  row_key_ = other.row_key_;
  return *this;
}

tigers::Model& tigers::Model::operator=(Model&& other) {
  ctx_ = std::move(other.ctx_);
  row_key_ = std::move(other.row_key_);
  return *this;
}

bool tigers::Model::HasAttribute(const std::string& attr_name) const {
  return attrs_.find(attr_name) != std::end(attrs_);
}

bool tigers::Model::HasRowKey() const {
  return !row_key_.empty();
}

const tigers::RowKey& tigers::Model::row_key() const {
  return row_key_;
}

void tigers::Model::set_row_key(const tigers::RowKey& key) {
  row_key_ = key;
}

void tigers::Model::set_ops_context(tigers::ops_context_t ctx) {
  ctx_ = ctx;
}

bool tigers::Model::bindAttr(
    const std::string& attr_name, 
    tigers::AttributeBinder binder) {
  bool has_attr = this->HasAttribute(attr_name);
  if (!has_attr) {
    this->attrs_.emplace(attr_name, binder);
  }
  return !has_attr;
}

tigers::AttributePair tigers::Model::attributes() const {
  tigers::AttributePair attr_vals;
  attr_vals.reserve(this->attrs_.size());

  for (auto const& kv : this->attrs_) {
    attr_vals.emplace_back(kv.first, kv.second);
  }
  return attr_vals;
}

namespace tigers {

tigers::AttributeBinder MakeStringBinder(std::reference_wrapper<std::string> s) {
  auto getter = [=]{ return s; };
  auto setter = [=](const std::string& value) { s.get() = value; };
  return std::make_pair(getter, setter);
};

AttributeGetter EmptyGetter() {
  return []{ return std::string(); };
}

AttributeSetter EmptySetter() {
  return [](const std::string& ) { };
}

std::string CallGet(const AttributeBinder& binder) {
  return binder.first();
}

std::string CallGet(std::reference_wrapper<const AttributeBinder> binder) {
  return CallGet(binder.get());
}

void CallSet(const AttributeBinder& binder, const std::string& value) {
  binder.second(value);
}

void CallSet(
    std::reference_wrapper<const AttributeBinder> binder, 
    const std::string& value) {
  CallSet(binder.get(), value);
}

} /* namespace tigers */
