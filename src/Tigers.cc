#include "Tigers.h"
#include "utils.h"

tigers::Tigers::Tigers(std::unique_ptr<rocksdb::DB> db, ColumnFamilyHandles cf_handles)
    : db_(std::move(db)), cf_handles_(std::move(cf_handles)) {

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

  std::vector<std::string> col_fam_names;
  std::vector<rocksdb::ColumnFamilyDescriptor> col_fam_desc {{rocksdb::kDefaultColumnFamilyName, rocksdb::ColumnFamilyOptions()}};
  std::vector<rocksdb::ColumnFamilyHandle*> col_fam_handles;

  auto status = rocksdb::DB::ListColumnFamilies(options, path, &col_fam_names);
  if (status.ok()) {
    for (auto const& col : col_fam_names) {
      col_fam_desc.emplace_back(col, rocksdb::ColumnFamilyOptions());
    }
  }

  rocksdb::DB* db;
  status = rocksdb::DB::Open(options, path, col_fam_desc, &col_fam_handles, &db);
  if (!status.ok()) return nullptr;

  ColumnFamilyHandles cf_handles;
  for (auto h : col_fam_handles) {
    cf_handles.emplace(
        h->GetName(), 
        std::unique_ptr<rocksdb::ColumnFamilyHandle>{h});
  }

  return tigers::make_unique<tigers::Tigers>(
      std::unique_ptr<rocksdb::DB>{db},
      std::move(cf_handles));
}

rocksdb::Options tigers::Tigers::DefaultOptions() {
  rocksdb::Options options;
  
  options.create_if_missing = true;
  // options.OptimizeLevelStyleCompaction();
  return options;
}

tigers::ops_context_t tigers::Tigers::getOrCreateOpsContext(
    const std::string& cf_name) {
  auto it = cf_handles_.find(cf_name);
  if (it == std::end(cf_handles_)) {
    rocksdb::ColumnFamilyHandle* cf_handle;
    auto status = db_->CreateColumnFamily(
        rocksdb::ColumnFamilyOptions(),
        cf_name,
        &cf_handle);
    if (!status.ok()) {
      return tigers::ops_context_t{nullptr, nullptr};
    }

    std::unique_ptr<rocksdb::ColumnFamilyHandle> cf{cf_handle};
    it = cf_handles_.emplace(cf_name, std::move(cf)).first;
  }
  return tigers::ops_context_t{db_.get(), it->second.get()};
}