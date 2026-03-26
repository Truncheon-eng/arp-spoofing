#include <unistd.h>
#include <map>
#include <string>
#include <thread>
#include <future>
#include <iostream>

#include "libpcapwrap.h"
#include "libnetwrap.h"
#include "discover.h"

using std::map;


std::string get_mac_address(char * dev, char * ip) {
    LibpcapWrapper p = LibpcapWrapper(dev);
    return p.run_loop(ip);
} 

int main(int argc, char * argv[]) {
    map<char, char *> params = parse_arguments(argc, argv);

    if (params.count('t') == 0) {
        std::cerr << RED << CROSS_MARK << "parameter \"t\" is not represented" << NORMAL << std::endl;
        exit(EXIT_FAILURE);
    }

    if (params.count('d') == 0) {
        std::cerr << RED << CROSS_MARK << "parameter \"d\" is not represented" << NORMAL << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // now we know that there are strings for parameters accordingly

    std::cout << YELLOW << INFO_MARK << "Target ip: " << params['t'] << NORMAL << std::endl;
    std::cout << YELLOW << INFO_MARK << "Iface name: " << params['d'] << NORMAL << std::endl;

    try {
        LibnetWrapper l{params['d']};

        std::future<std::string> future = std::async(
            std::launch::async,
            get_mac_address,
            params['d'],
            params['t']
        );

        std::future_status status;
        std::string mac_address;

        while ((status = future.wait_for(std::chrono::milliseconds(1000))) != std::future_status::ready)
            l.make_arp_request(params['t']);
            
        mac_address = future.get();

        std::cout << YELLOW << INFO_MARK << mac_address << NORMAL << std::endl;
    } catch (LibnetException& err) {
        std::cerr << err.getMessage() << std::endl;
        exit(EXIT_FAILURE);
    } catch (LibpcapException& err) {
        std::cerr << err.getMessage() << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}