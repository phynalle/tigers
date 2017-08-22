#ifndef TIGERS_QUERY_H_
#define TIGERS_QUERY_H_

#include <vector>
#include <string>
#include <rocksdb/db.h>

#include "types.h"

namespace tigers {

class Tigers;

template <class T>
class Query {
 public:
  Query(Query &&) = default;
  Query(const Query &) = default;
  Query &operator=(Query &&) = default;
  Query &operator=(const Query &) = default;
  ~Query() = default;

  std::vector<T> Scan(const std::string& start, const std::string& end, int size);
  bool Put(const std::string& key, T& model);
  bool Get(const std::string& key, T& model);
  bool Update(const T& model);
  bool Delete(const T& model);
  bool Exists(const std::string& key);
  
 private:
  explicit Query(ops_context_t ctx);

  ops_context_t ctx_;
  friend class Tigers;
};

} /* tigers */

#include "Query.inl"

#endif /* TIGERS_QUERY_H_ */