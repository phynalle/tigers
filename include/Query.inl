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
bool tigers::Query<T>::Put(const std::string& row_key, T& model) {
  if (ctx_.IsEmpty()) return false;

  rocksdb::WriteBatch batch;
  for (auto const& attr : model.attributes()) {
    std::string key = row_key + '.' + attr.first;
    batch.Put(ctx_.cf_handle, key, CallGet(attr.second));
  }

  bool ok = ctx_.db->Write(rocksdb::WriteOptions(), &batch).ok();
  if (ok) {
    model.set_row_key(row_key);
  }
} 

template <class T>
bool tigers::Query<T>::Get(const std::string& row_key, T& model) {
  if (ctx_.IsEmpty()) return false;

  T obj {ctx_};
  for (auto const& attr : model.attributes()) {
    std::string key = row_key + '.' + attr.first, value;
    auto s = ctx_.db->Get(rocksdb::ReadOptions(), ctx_.cf_handle, key, &value);
    if (!s.ok()) return false;

    CallSet(attr.second, value);
  }

  model.set_row_key(row_key);
  model = obj;
  return true;
} 

template <class T>
bool tigers::Query<T>::Update(const T& model) {
  if (ctx_.IsEmpty()) return false;
  return false;
} 

template <class T>
bool tigers::Query<T>::Delete(const T& model) {
  if (ctx_.IsEmpty()) return false;
  return false;
} 

template <class T>
bool tigers::Query<T>::Exists(const std::string& key) {
  if (ctx_.IsEmpty()) return false;

} 

#endif /* TIGERS_QUERY_INL */