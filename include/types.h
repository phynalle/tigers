#ifndef TIGERS_TYPES_H_
#define TIGERS_TYPES_H_ 

#include <string>
#include <rocksdb/db.h>

namespace tigers {

using RowKey = std::string;
using Key = std::string;
using Value = std::string;
using AttributeFamilyName = std::string;

using AttributeGetter = std::function<std::string()>;
using AttributeSetter = std::function<void(const std::string&)>;
using AttributeBinder = std::pair<AttributeGetter, AttributeSetter>;
using AttributePair = std::vector<std::pair<
    std::string, 
    std::reference_wrapper<const AttributeBinder
>>>;

struct ops_context_t {
  rocksdb::DB* db;
  rocksdb::ColumnFamilyHandle* cf_handle;

  bool IsEmpty() const { return db == nullptr || cf_handle == nullptr; }
};

} /* namespace tigers */

#endif /* ifndef TIGERS_TYPES_H_ */
