#include <Shared/Simulation.hh>
#include <Server/Server.hh>

#include <iostream>

int main() {
    std::cout << "Diagnostics: {\n";
    std::cout << "  Simulation Size: " << sizeof(Simulation) << '\n';
    std::cout << "  Spatial Hash Size: " << sizeof(SpatialHash) << '\n';
    std::cout << "  Entity Size: " << sizeof(Entity) << '\n';
    std::cout << "}\n";
    Server::init();
    return 0;
}

