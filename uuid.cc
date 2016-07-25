#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

int main() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string uuid_str = boost::lexical_cast<std::string>(uuid);
    boost::replace_all(uuid_str,"-","");
    std::cout << uuid_str << std::endl;
}
