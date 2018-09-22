#pragma once

#include "rocksdb/table.h"
#include "utilities/titandb/blob_file_manager.h"
#include "utilities/titandb/options.h"

namespace rocksdb {
namespace titandb {

class TitanTableFactory : public TableFactory {
 public:
  TitanTableFactory(const TitanCFOptions& options,
                    std::shared_ptr<BlobFileManager> blob_manager)
      : options_(options),
        base_factory_(options.table_factory),
        blob_manager_(blob_manager) {}

  //const char* Name() const override { return "TitanTable"; }
  const char* Name() const override { return "BlockBasedTable"; }

  Status NewTableReader(
      const TableReaderOptions& options,
      std::unique_ptr<RandomAccessFileReader>&& file, uint64_t file_size,
      std::unique_ptr<TableReader>* result,
      bool prefetch_index_and_filter_in_cache = true) const override;

  TableBuilder* NewTableBuilder(const TableBuilderOptions& options,
                                uint32_t column_family_id,
                                WritableFileWriter* file) const override;

  std::string GetPrintableTableOptions() const override;

  Status SanitizeOptions(const DBOptions& db_options,
                         const ColumnFamilyOptions& cf_options) const override {
    // Override this when we need to validate our options.
    return base_factory_->SanitizeOptions(db_options, cf_options);
  }

  Status GetOptionString(std::string* opt_string,
                         const std::string& delimiter) const override {
    // Override this when we need to persist our options.
    return base_factory_->GetOptionString(opt_string, delimiter);
  }

  void* GetOptions() override { return base_factory_->GetOptions(); }

  bool IsDeleteRangeSupported() const override {
    return base_factory_->IsDeleteRangeSupported();
  }

 private:
  TitanCFOptions options_;
  std::shared_ptr<TableFactory> base_factory_;
  std::shared_ptr<BlobFileManager> blob_manager_;
};

}  // namespace titandb
}  // namespace rocksdb
