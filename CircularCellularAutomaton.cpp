#ifndef CIRCULAR_CLLULAR_AUTOMATON
#define CIRCULAR_CLLULAR_AUTOMATON

#include <vector>
#include <string>
#include <iostream>

std::string to_string(std::vector<unsigned> vec)
{
    std::string str;
    str += "{";
    for(auto it=vec.cbegin() ; it<vec.cend()-1 ; ++it)
    {
        ;
        str += std::to_string(*it) + std::string(", ");
    }
    str += std::to_string(vec.back());
    str += "}";
    return str;
}

class CircularCellularAutomaton
{
    public:
        CircularCellularAutomaton(
            const unsigned              automatonOrder,
            const unsigned              cellsOrder,
            const unsigned              environmentDistance,
            const std::vector<unsigned> initialValues)
            : automatonOrder(automatonOrder),
              cellsOrder(cellsOrder),
              environmentDistance(environmentDistance)
            {
                this->values = initialValues;
                return;
            }
        operator std::string() const
        {
            std::string str;
            str += std::string("Circular Cellular Automaton")                                         + std::string("\n") + \
                   std::string("---------------------------")                                         + std::string("\n") + \
                   std::string("automaton order      = ") + std::to_string(this->automatonOrder)      + std::string("\n") + \
                   std::string("cells order          = ") + std::to_string(this->cellsOrder)          + std::string("\n") + \
                   std::string("environment distance = ") + std::to_string(this->environmentDistance) + std::string("\n") + \
                   std::string("values               = ") + to_string(this->values)                   + std::string("\n") + \
                   std::string("---------------------------")                                         + std::string("\n") ;
            return str;
        }

    private:
        const unsigned        automatonOrder;
        const unsigned        cellsOrder;
        const unsigned        environmentDistance;
        std::vector<unsigned> values;
};

int main ()
{
    CircularCellularAutomaton automaton(5, 3, 2, {0, 1, 2, 0, 1});
    std::cout << std::string(automaton) << std::endl;
    return 0;
}

#endif
