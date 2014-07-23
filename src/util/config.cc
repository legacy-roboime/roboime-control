#include "config.h"

#include <fstream>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

namespace roboime
{
    config::config()
    {
        try
        {
            boost::property_tree::json_parser::read_json("config.json", data_);
        }
        catch (std::exception& e)
        {
            std::cout << __FUNCTION__ << " [ERROR] " << e.what() << std::endl;
            throw;
        }
    }

    std::string 
    config::get(const std::string& s) const
    {
        std::string ret;
        try
        {
            ret = data_.get<std::string>(s);
        }
        catch (std::exception& e)
        {
            std::cout << "ERROR: " << e.what() << std::endl;
        }
        return ret;
    }
}
