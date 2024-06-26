#include "utils/util.h"
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <fstream>
int main()
{
    std::cout << "curpath:: " << cero::getCurPath() << std::endl;

    Json::Reader rd;
    Json::Value root;
    std::ifstream ifs("../bin/test.json");
    rd.parse(ifs, root);

    if (root.isObject())
    {
        std::string name = root["name"].asString();

        std::cout << name << std::endl;
    }
    else
    {
        std::cout << "no parser." << std::endl;
    }
    return 0;
}