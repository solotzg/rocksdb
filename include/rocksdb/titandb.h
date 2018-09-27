#pragma once

#include "utilities/titandb/db.h"
#include "utilities/titandb/options.h"

namespace rocksdb {
namespace titandb {

static Status Open(const Options& options,
                   const std::string& name,
                   DB** dbptr) {
  TitanOptions titan_ops(options);
  return TitanDB::Open(titan_ops, name, dbptr);
}

static Status Open(const DBOptions& db_options, const std::string& name,
                   const std::vector<ColumnFamilyDescriptor>& column_families,
                   std::vector<ColumnFamilyHandle*>* handles, DB** dbptr) {
  TitanDBOptions titan_ops(db_options);
  std::vector<TitanCFDescriptor> titan_cf_descr;
  for (auto c: column_families) {
    TitanCFOptions titan_cf_cfg(c.options);
    titan_cf_cfg.min_blob_size = 0;
    TitanCFDescriptor t(c.name, titan_cf_cfg);
    titan_cf_descr.push_back(t);
  }

  return TitanDB::Open(titan_ops, name, titan_cf_descr, handles, dbptr);
}

}
}