#include "FileHelper.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

namespace meta_log {

FileHelper::~FileHelper() { Close(); }

bool FileHelper::Open(const std::string& file_name, const std::string& mode)
{
    Close();
    file_name_ = file_name;

    std::string dir_name = DirectoryName(file_name);
    if (!DirectoryExists(dir_name))
    {
        CreateDirectory(dir_name);
    }

    fd_ = fopen(file_name.c_str(), mode.c_str());

    return fd_ != nullptr;
}

void FileHelper::Flush()
{
    std::lock_guard<std::mutex> lock_guard(write_mtx_);
    std::fflush(fd_);
}

void FileHelper::Close()
{
    if (fd_ != nullptr)
    {
        std::fclose(fd_);
        fd_ = nullptr;
    }
}

void FileHelper::Write(const void* data_ptr, std::size_t n)
{
    std::lock_guard<std::mutex> lock_guard(write_mtx_);
    fwrite(data_ptr, 1, n, fd_);
}

std::size_t FileHelper::Size() const
{
    struct stat st;
    if (stat(file_name_.c_str(), &st) == 0)
    {
        return static_cast<std::size_t>(st.st_size);
    }
    return 0;
}

const std::string& FileHelper::FileName() const { return file_name_; }

bool FileHelper::DirectoryExists(const std::string& path)
{
    struct stat st = {0};
    if (stat(path.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool FileHelper::CreateDirectory(const std::string& path)
{
    std::string mkdir_cmd = "mkdir -p " + path;
    return 0 == system(mkdir_cmd.c_str());
}

std::string FileHelper::DirectoryName(const std::string& path)
{
    auto pos = path.find_last_of('/');
    return pos != std::string::npos ? path.substr(0, pos) : std::string();
}

std::string FileHelper::DirectoryWithSlash(const std::string& path)
{
    size_t len = path.size();
    if (len > 0 && path[len - 1] != '/')
    {
        return path + "/";
    }
    return path;
}

std::string FileHelper::GetFileNameWithIndex(const std::string& file_name, int index)
{
    if (index > 0)
    {
        auto pos = file_name.rfind('.');
        if (pos != std::string::npos && pos != 0 && pos != file_name.size() - 1)
        {
            char buff[256];
            int buff_writen_size = snprintf(buff, sizeof buff, "%s.%d.%s", file_name.substr(0, pos).c_str(), index,
                                            file_name.substr(pos + 1).c_str());
            return std::string(buff, buff_writen_size);
        }
    }
    return file_name;
}

std::string FileHelper::GetFileNameWithTs(time_t ts, const std::string& file_suffix)
{
    tm ltm = {0};
    localtime_r(&ts, &ltm);
    char buff[1024];
    int buff_writen_size =
        snprintf(buff, sizeof buff, "%04d-%02d-%02d %02d:%02d:%02d.%s", ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
                 ltm.tm_hour, ltm.tm_min, ltm.tm_sec, file_suffix.c_str());
    return std::string(buff, buff_writen_size);
}
}  // namespace meta_log