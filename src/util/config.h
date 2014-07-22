#include <vector>
#include <boost/property_tree/ptree.hpp>

#define ERROR(e) do { std::cerr << __FUNCTION__ << __LINE__ << " [ERROR] " << e.what() << std::endl; } while(0)
#define INFO(e) do { std::cerr << __FUNCTION__ << __LINE__ << " [INFO] " << e.what() << std::endl; } while(0)
#define WARN(e) do { std::cerr << __FUNCTION__ << __LINE__ << " [WARN] " << e.what() << std::endl; } while(0)

namespace roboime
{
    class config
    {
        private:
            boost::property_tree::ptree data_;
        public:
            config();
            std::string get(const std::string& key_sequence) const;
    };
}
