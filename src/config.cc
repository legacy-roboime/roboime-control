#include "config.h"

#include <fstream>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

namespace roboime
{
    config::config()
    {
        boost::property_tree::json_parser::read_json("config.json", data_);
    }
}
