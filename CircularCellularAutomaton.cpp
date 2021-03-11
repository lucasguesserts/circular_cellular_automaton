#ifndef CIRCULAR_CLLULAR_AUTOMATON
#define CIRCULAR_CLLULAR_AUTOMATON

#include <vector>
#include <string>
#include <iostream>

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
    private:
        const unsigned        automatonOrder;
        const unsigned        cellsOrder;
        const unsigned        environmentDistance;
        std::vector<unsigned> values;
};

int main ()
{
    return 0;
}

#endif
