#include <Shared/Simulation.hh>
#include <Server/Server.hh>

#include <iostream>


int main() {
    std::cout << "Diagnostics: {\n";
    std::cout << "  Server Size: " << sizeof(Server) << '\n';
    std::cout << "  Simulation Size: " << sizeof(Simulation) << '\n';
    std::cout << "  Entity Size: " << sizeof(Entity) << '\n';
    std::cout << "}\n";
    static Server server;
    server.run();
    
    return 0;
}