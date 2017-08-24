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

  std::vector<T> Scan(const std::string& start, const std::string& end, int size);
  rocksdb::Status Put(const std::string& key, T& model);
  rocksdb::Status Get(const std::string& key, T& model);
  rocksdb::Status Update(const T& model);
  rocksdb::Status Delete(const T& model);
  bool Exists(const std::string& key);
  
 private:
  explicit Query(ops_context_t ctx);

  ops_context_t ctx_;

  friend class Tigers;
};

} /* tigers */

#include "Query.inl"

#endif /* TIGERS_QUERY_H_ */