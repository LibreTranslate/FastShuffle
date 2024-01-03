#include "shuffle.hpp"

mmap_handle_t mmap_open(const std::string &file){
    mmap_handle_t mht;

#if defined(_WIN32) || defined(_WIN64)
    mht.file_handle = CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (mht.file_handle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Unable to open " + file);
    }

    LARGE_INTEGER li;
    GetFileSizeEx(mht.file_handle, &li);
    mht.file_size = li.QuadPart;
    mht.file_mapping = CreateFileMapping(mht.file_handle, NULL, PAGE_READONLY, 0, 0, NULL);
    if (mht.file_mapping == NULL) {
        CloseHandle(mht.file_handle);
        throw std::runtime_error("File mapping failed.");
    }

    mht.file_data = static_cast<const char*>(MapViewOfFile(mht.file_mapping, FILE_MAP_READ, 0, 0, mht.file_size));
    if (mht.file_data == NULL) {
        CloseHandle(mht.file_mapping);
        CloseHandle(mht.file_handle);
        throw std::runtime_error("Memory mapping failed.");
    }
#else
    mht.fd = open(file.c_str(), O_RDONLY);
    if (mht.fd == -1) throw std::runtime_error("Unable to open " + file);

    struct stat file_stat;
    if (fstat(mht.fd, &file_stat) == -1) {
        close(mht.fd);
        throw std::runtime_error("Unable to get file size for " + file);
    }

    mht.file_size = file_stat.st_size;
    mht.file_data = static_cast<const char*>(mmap(NULL, mht.file_size, PROT_READ, MAP_PRIVATE, mht.fd, 0));
    if (mht.file_data == MAP_FAILED) {
        close(mht.fd);
        throw std::runtime_error("Memory mapping failed.");
    }

#endif
    return mht;
}

void mmap_close(mmap_handle_t mht){
#if defined(_WIN32) || defined(_WIN64)
    UnmapViewOfFile(mht.file_data);
    CloseHandle(mht.file_mapping);
    CloseHandle(mht.file_handle);
#else
    munmap(const_cast<char*>(mht.file_data), mht.file_size);
    close(mht.fd);
#endif
}

size_t line_count(mmap_handle_t mht){
    size_t count = 0;
    const char* line_start = mht.file_data;
    const char* line_end = mht.file_data;
    for (const char* ptr = mht.file_data; ptr < mht.file_data + mht.file_size; ++ptr) {
        if (*ptr == '\n' || ptr == mht.file_data + mht.file_size - 1) {
            line_end = ptr + 1;
            line_start = line_end;
            count++;
        }
    }
    return count;
}

std::tuple<std::string, std::string> shuffle(const std::string &src, const std::string &tgt){
    mmap_handle_t smht = mmap_open(src);
    mmap_handle_t tmht = mmap_open(tgt);

    std::string src_out = src + ".shuffled";
    std::string tgt_out = tgt + ".shuffled";

    // size_t src_count = line_count(smht);

    std::vector<line_off_t> offsets;
    
    line_off_t l;
    l.src_start = smht.file_data;
    l.src_end = smht.file_data;
    
    // Scan source
    for (const char* s_ptr = smht.file_data; s_ptr < smht.file_data + smht.file_size; ++s_ptr) {
        if (*s_ptr == '\n' || s_ptr == smht.file_data + smht.file_size - 1) {
            int pad = *(s_ptr - 1) == '\r' ? 1 : 0;
            l.src_end = s_ptr - pad;
            offsets.push_back(l);
            l.src_start = l.src_end + pad + 1;
        }
    }

    if (offsets.size() == 0){
        throw std::runtime_error("Source is empty");
    }

    offsets[0].tgt_start = tmht.file_data;
    offsets[0].tgt_end = tmht.file_data;

    // Scan target
    size_t i = 0;
    for (const char* t_ptr = tmht.file_data; t_ptr < tmht.file_data + tmht.file_size; ++t_ptr) {
        if (*t_ptr == '\n' || t_ptr == tmht.file_data + tmht.file_size - 1) {
            int pad = *(t_ptr - 1) == '\r' ? 1 : 0;
            offsets[i].tgt_end = t_ptr - pad;
            offsets[i+1].tgt_start = offsets[i].tgt_end + pad + 1;
            i++;
        }
    }

    // Shuffle
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(std::begin(offsets), std::end(offsets), e);

    // Write out
    std::ofstream src_of(src_out, std::ios::trunc);
    std::ofstream tgt_of(tgt_out, std::ios::trunc);

    // std::cout << offsets.size() << std::endl << std::endl;
    for (size_t i = 0; i < offsets.size(); i++){
        src_of.write(offsets[i].src_start, offsets[i].src_end - offsets[i].src_start);
        src_of.write("\n", 1);
        tgt_of.write(offsets[i].tgt_start, offsets[i].tgt_end - offsets[i].tgt_start);
        tgt_of.write("\n", 1);
    }

    src_of.close();
    tgt_of.close();

    mmap_close(smht);
    mmap_close(tmht);

    return std::make_tuple(src_out, tgt_out);
}