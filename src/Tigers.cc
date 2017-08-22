#include "Tigers.h"
#include "utils.h"

tigers::Tigers::Tigers(std::unique_ptr<rocksdb::DB> db)
    : db_(std::move(db)) {

}

tigers::Tigers::~Tigers() {

}
// 
// tigers::Query tigers::Tigers::Query(const std::string& af_name) {
//   return tigers::Query{*this, af_name};
// }

std::unique_ptr<tigers::Tigers> tigers::Tigers::Open(
    const std::string& path, 
    const rocksdb::Options& options) {
  rocksdb::DB* db;
  auto status = rocksdb::DB::Open(options, path, &db);

  if (!status.ok()) return nullptr;
  return tigers::make_unique<tigers::Tigers>(std::unique_ptr<rocksdb::DB>{db});
}

rocksdb::Options tigers::Tigers::DefaultOptions() {
  rocksdb::Options options;
  
  options.create_if_missing = true;
  // options.OptimizeLevelStyleCompaction();
  return options;
}

tigers::ops_context_t tigers::Tigers::getOpsContext(const std::string& cf_name) {
  auto it = this->model_handles_.find(cf_name);
  if (it == std::end(this->model_handles_)) {
    return tigers::ops_context_t{nullptr, nullptr};
  }
  return tigers::ops_context_t{db_.get(), it->second.get()};
}