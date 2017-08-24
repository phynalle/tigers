#ifndef TIGERS_TIGERS_H_
#define TIGERS_TIGERS_H_

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <rocksdb/db.h>
#include <rocksdb/options.h>

#include "Query.h"
#include "types.h"

namespace tigers {

using ColumnFamilyHandles = std::unordered_map<
    std::string, 
    std::unique_ptr<rocksdb::ColumnFamilyHandle>>;

class Tigers {
 public:
  explicit Tigers(std::unique_ptr<rocksdb::DB> db, ColumnFamilyHandles cf_handles);
  Tigers(const Tigers &) = delete;
  Tigers(Tigers &&) = delete;
  Tigers& operator=(const Tigers &) = delete;
  Tigers& operator=(Tigers &&) = delete;
  ~Tigers();

  static std::unique_ptr<Tigers> Open(const std::string& path, const rocksdb::Options& options);
  static std::unique_ptr<Tigers> Open(const std::string& path) {
    return Open(path, DefaultOptions());
  }
  static rocksdb::Options DefaultOptions();

  template <typename T>
  Query<T> Querier() {
    auto ctx = this->getOrCreateOpsContext(T::model_name);
    return Query<T>{ctx};
  }
  
 private:
  ops_context_t getOrCreateOpsContext(const std::string& cf_name);

  std::unique_ptr<rocksdb::DB> db_;
  ColumnFamilyHandles cf_handles_; 
  std::mutex mutex_;
};

} /* namespace tigers */

#endif /* ifndef TIGERS_TIGERS_H_ */
