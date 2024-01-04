#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstring>
#include <tuple>
#include <variant>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

typedef struct{
    const char *file_data;
    #if defined(_WIN32) || defined(_WIN64)
    HANDLE file_handle;
    HANDLE file_mapping;
    SIZE_T file_size;
    #else
    int fd;
    off_t file_size;
    #endif
} mmap_handle_t;


typedef struct{
    const char* src_start;
    const char* src_end;
    const char* tgt_start;
    const char* tgt_end; 
} line_off_t;

mmap_handle_t mmap_open(const std::string &file);
void mmap_close(mmap_handle_t mht);
size_t line_count(mmap_handle_t mht);
std::tuple<std::string, std::string> shuffle(const std::string &src, const std::string &tgt);
std::tuple<std::string, std::string, std::string, std::string> shuffle_sample(const std::string &src, const std::string &tgt, long long sample);
