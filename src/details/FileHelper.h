#pragma once

#include <mutex>
#include <string>

namespace meta_log {
class FileHelper
{
  public:
    explicit FileHelper() = default;

    FileHelper(const FileHelper&) = delete;
    FileHelper& operator=(const FileHelper&) = delete;

    ~FileHelper();

    bool Open(const std::string& file_name, const std::string& mode);
    void Flush();
    void Close();
    void Write(const void* data, std::size_t n);
    std::size_t Size() const;
    const std::string& FileName() const;

    static bool DirectoryExists(const std::string& path);
    static bool CreateDirectory(const std::string& path);
    static std::string DirectoryName(const std::string& path);
    static std::string DirectoryWithSlash(const std::string& path);

    static std::string GetFileNameWithIndex(const std::string& file_name, int index);
    static std::string GetFileNameWithTs(time_t ts, const std::string& file_suffix);

  private:
    std::FILE* fd_{nullptr};
    std::string file_name_;
    std::mutex write_mtx_;
};

}  // namespace meta_log