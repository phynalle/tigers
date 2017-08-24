#ifndef TIGERS_QUERY_INL
#define TIGERS_QUERY_INL

#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

#include "Query.h"

template <class T>
tigers::Query<T>::Query(ops_context_t ctx)
    : ctx_(ctx) {

}

template <class T>
std::vector<T> tigers::Query<T>::Scan(const std::string& start, const std::string& end, int size) {
  std::vector<T> rows;
  if (!ctx_.IsEmpty()) {

  }
  return rows;
} 

template <class T>
rocksdb::Status tigers::Query<T>::Put(const std::string& row_key, T& model) {
  auto status = putAttributes(row_key, model);
  if (status.ok()) {
    set_row_key(model, row_key);
  }
  return status;
} 

template <class T>
rocksdb::Status tigers::Query<T>::Get(const std::string& row_key, T& model) {
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  T obj;
  this->set_ops_context(obj, ctx_);
  for (auto const& attr : attributes(obj)) {
    std::string key = row_key + '.' + attr.first, value;
    auto s = ctx_.db->Get(rocksdb::ReadOptions(), ctx_.cf_handle, key, &value);
    if (!s.ok()) return s;

    CallSet(attr.second, value);
  }

  this->set_row_key(obj, row_key);
  model = std::move(obj);
  return rocksdb::Status::OK();
} 

template <class T>
rocksdb::Status tigers::Query<T>::Update(const T& model) {
  if (!model.HasRowKey()) return rocksdb::Status::InvalidArgument();
  return putAttributes(model.row_keys(), model);
} 

template <class T>
rocksdb::Status tigers::Query<T>::Delete(const T& model) {
  if (!model.HasRowKey()) return rocksdb::Status::InvalidArgument();
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  rocksdb::WriteBatch batch;
  for (auto const& attr : attributes(model)) {
    std::string key = model.row_key() + '.' + attr.first;
    batch.Delete(ctx_.cf_handle, key);
  }

  return ctx_.db->Write(rocksdb::WriteOptions(), &batch);
} 

template <class T>
bool tigers::Query<T>::Exists(const std::string& key) {
  if (ctx_.IsEmpty()) return false;
  return false;
} 

template <class T>
rocksdb::Status tigers::Query<T>::putAttributes(const std::string& row_key, const 
  T& model) {
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  rocksdb::WriteBatch batch;
  for (auto const& attr : attributes(model)) {
    std::string key = row_key + '.' + attr.first;
    batch.Put(ctx_.cf_handle, key, CallGet(attr.second));
  }

  return ctx_.db->Write(rocksdb::WriteOptions(), &batch);
} 

#endif /* TIGERS_QUERY_INL */