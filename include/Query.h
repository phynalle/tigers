#ifndef TIGERS_QUERY_H_
#define TIGERS_QUERY_H_

#include <vector>
#include <string>
#include <rocksdb/db.h>

#include "types.h"
#include "Model.h"

namespace tigers {

class Tigers;

template <class T>
class Query : public Model::Accessor {
 public:
  Query(Query &&) = default;
  Query(const Query &) = default;
  Query &operator=(Query &&) = default;
  Query &operator=(const Query &) = default;
  ~Query() = default;

  rocksdb::Status ScanIndex(
      const std::string& start,
      std::vector<std::string>& results,
      size_t max=0) const;
  rocksdb::Status ScanIndex
  (std::vector<std::string>& results, size_t max=0) const {
    return ScanIndex(std::string{}, results, max);
  }

  rocksdb::Status Put(const std::string& key, T& model);
  rocksdb::Status Get(const std::string& key, T& model) const;
  rocksdb::Status MultiGet(
      const std::vector<std::string>& keys,
      std::vector<std::unique_ptr<T>>& models) const;
  rocksdb::Status Update(const T& model);
  rocksdb::Status Delete(const T& model);
  bool Exists(const std::string& key) const;
  
 private:
  explicit Query(ops_context_t ctx);
  void putAttributes(
      const std::string& key, 
      const T& model,
      rocksdb::WriteBatch&) const;

  std::string getIndexKey(const std::string& row_key) const;
  rocksdb::DB* getDB() const;
  rocksdb::ColumnFamilyHandle* getHandle() const;

  ops_context_t ctx_;

  friend class Tigers;
};

} /* tigers */

#include "Query.inl"

#endif /* TIGERS_QUERY_H_ */