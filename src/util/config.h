#ifndef __ROBOIME_CONFIG_H
#define __ROBOIME_CONFIG_H

#include <vector>
#include <boost/property_tree/ptree.hpp>

#define ERROR(e) do { std::cerr << __FUNCTION__ << " " << __LINE__ << " [ERROR] " << e.what() << std::endl; } while(0)
#define INFO(e) do { std::cerr << __FUNCTION__ << " " << __LINE__ << " [INFO] " << e.what() << std::endl; } while(0)
#define WARN(e) do { std::cerr << __FUNCTION__ << " " << __LINE__ << " [WARN] " << e.what() << std::endl; } while(0)

namespace roboime
{
    class config
    {
        private:
            boost::property_tree::ptree data_;
        public:
            config();
            template<typename T>
            T get(const std::string& key) const
            {
                return data_.get<T>(key);
            }
    };
}
#endif
