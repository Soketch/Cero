#include <unistd.h>
#include <experimental/filesystem> //c++17文件系统
#include <string>

namespace fs = std::experimental::filesystem;

namespace cero
{
    std::string getCurPath();
}