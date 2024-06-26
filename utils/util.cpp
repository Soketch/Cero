#include "util.h"

namespace cero
{
    std::string getCurPath()
    {
        fs::path curPath = fs::current_path();
        return curPath;
    }
}