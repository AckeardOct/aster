#include "file_system.h"

#include "common/logger.h"

#include <filesystem>
#include <fstream>
#include <sstream>

bool fs::exists(StringRef filePath)
{
    return std::filesystem::exists(filePath);
}

String fs::cat(StringRef filePath)
{
    ASSERT_MSG(fs::exists(filePath), "file not found: %s", filePath.data());

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filePath.data());
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    } catch (std::ifstream::failure e) {
        ASSERT_FAIL("Can't read file: %s", filePath.data());
        return "";
    }
}
