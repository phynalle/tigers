#ifndef TIGERS_QUERY_INL
#define TIGERS_QUERY_INL

#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

#include "Query.h"
#include "utils.h"

namespace tigers {
  static const std::string kIndexPrefix = "__index__";
}

template <class T>
tigers::Query<T>::Query(ops_context_t ctx)
    : ctx_(ctx) {

}

template <class T>
rocksdb::Status tigers::Query<T>::ScanIndex(
    const std::string& start,
    std::vector<std::string>& results,
    size_t max) const {
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  std::string prefix = kIndexPrefix + ".__key__.";
  std::vector<T> rows;
  if (!ctx_.IsEmpty()) {
    std::unique_ptr<rocksdb::Iterator> it{
      getDB()->NewIterator(rocksdb::ReadOptions(), getHandle())
    };

    size_t count = 0;
    for (it->Seek(prefix + start); it->Valid(); it->Next()) {
      if (!it->key().starts_with(prefix))
        break;

      results.emplace_back(it->value().ToString());
      if (max > 0 && ++count == max)
        break;
    }
  }

  return rocksdb::Status::OK();
} 

template <class T>
rocksdb::Status tigers::Query<T>::Put(const std::string& row_key, T& model) {
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  rocksdb::WriteBatch batch;
  putAttributes(row_key, model, batch);
  batch.Put(getHandle(), getIndexKey(row_key), row_key);

  auto status = getDB()->Write(rocksdb::WriteOptions(), &batch);
  if (status.ok()) {
    set_row_key(model, row_key);
  }
  return status;
} 

template <class T>
rocksdb::Status tigers::Query<T>::Get(const std::string& row_key, T& model) const {
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  T obj;
  this->set_ops_context(obj, ctx_);
  for (auto const& attr : attributes(obj)) {
    std::string key = row_key + '.' + attr.first, value;
    auto s = getDB()->Get(rocksdb::ReadOptions(), getHandle(), key, &value);
    if (!s.ok()) return s;

    CallSet(attr.second, value);
  }

  this->set_row_key(obj, row_key);
  model = std::move(obj);
  return rocksdb::Status::OK();
} 

template <class T>
rocksdb::Status tigers::Query<T>::MultiGet(
    const std::vector<std::string>& keys,
    std::vector<std::unique_ptr<T>>& models) const {
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  for (auto const& key : keys) {
    auto obj = tigers::make_unique<T>();
    if (Get(key, *obj).ok()) {
      models.emplace_back(std::move(obj));
    }
  }
  return rocksdb::Status::OK();
}

template <class T>
rocksdb::Status tigers::Query<T>::Update(const T& model) {
  if (!model.HasRowKey()) return rocksdb::Status::InvalidArgument();
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  rocksdb::WriteBatch batch;
  putAttributes(model.row_keys(), model);
  return getDB()->Write(rocksdb::WriteOptions(), &batch);
} 

template <class T>
rocksdb::Status tigers::Query<T>::Delete(const T& model) {
  if (!model.HasRowKey()) return rocksdb::Status::InvalidArgument();
  if (ctx_.IsEmpty()) return rocksdb::Status::NotFound();

  rocksdb::WriteBatch batch;
  batch.Delete(getIndexKey(model.row_key()));
  for (auto const& attr : attributes(model)) {
    std::string key = model.row_key() + '.' + attr.first;
    batch.Delete(getHandle(), key);
  }

  return getDB()->Write(rocksdb::WriteOptions(), &batch);
} 

template <class T>
bool tigers::Query<T>::Exists(const std::string& key) const {
  if (ctx_.IsEmpty()) return false;
  return false;
} 

template <class T>
void tigers::Query<T>::putAttributes(
      const std::string& row_key, 
      const T& model,
      rocksdb::WriteBatch& batch) const {
  for (auto const& attr : attributes(model)) {
    std::string key = row_key + '.' + attr.first;
    batch.Put(getHandle(), key, CallGet(attr.second));
  }
} 

template <class T>
std::string tigers::Query<T>::getIndexKey(const std::string& row_key) const {
  return tigers::kIndexPrefix + ".__key__." + row_key;
}

template <class T>
rocksdb::DB* tigers::Query<T>::getDB() const {
  return ctx_.db;
}

template <class T>
rocksdb::ColumnFamilyHandle* tigers::Query<T>::getHandle() const {
  return ctx_.cf_handle;
}

  
#endif /* TIGERS_QUERY_INL */