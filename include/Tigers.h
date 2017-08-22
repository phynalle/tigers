#ifndef TIGERS_TIGERS_H_
#define TIGERS_TIGERS_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <rocksdb/db.h>
#include <rocksdb/options.h>

#include "Query.h"
#include "types.h"

namespace tigers {

class Tigers {
 public:
  explicit Tigers(std::unique_ptr<rocksdb::DB> db);
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
  Query<T> Query() {
    return getColumnFamilyHandleOrCreate(T::model_name);
  }
  
  // auto tigers.Query<Model>();

  // Query Query(const std::string& af_name);

 private:
  ops_context_t getOpsContext(const std::string& cf_name);

  std::unique_ptr<rocksdb::DB> db_;
  std::unordered_map<
      std::string, 
      std::unique_ptr<rocksdb::ColumnFamilyHandle>>
    model_handles_;

};

} /* namespace tigers */

#endif /* ifndef TIGERS_TIGERS_H_ */
