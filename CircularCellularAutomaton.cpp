#ifndef CIRCULAR_CLLULAR_AUTOMATON
#define CIRCULAR_CLLULAR_AUTOMATON

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>

std::string to_string(std::vector<unsigned> vec)
{
    std::string str = "";
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
                if (initialValues.size() != automatonOrder)
                    throw std::invalid_argument("automaton order and amount of initial values differ");
                else
                    this->values = initialValues;
                return;
            }
        void step(void)
        {
            auto newValues = this->values;
            for(unsigned cell=0 ; cell<this->automatonOrder ; ++cell)
            {
                newValues[cell] = this->computeNewValue(cell);
            }
            this->values = newValues;
            return;
        }
        unsigned computeNewValue(const unsigned cell)
        {
            unsigned accumulator = 0;
            for(unsigned otherCell=0 ; otherCell<this->automatonOrder ; ++otherCell)
            {
                if (this->computeDistance(cell, otherCell) < this->environmentDistance)
                {
                    accumulator += this->values[otherCell];
                }
            }
            return accumulator;
        }
        unsigned computeDistance(const unsigned lhs, const unsigned rhs)
        {
            return std::fmin(
                std::fabs(lhs - rhs),
                this->automatonOrder - std::fabs(lhs - rhs)
            );
        }
        operator std::string() const
        {
            std::string str = "";
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
    for(unsigned k=0 ; k<5 ; ++k)
    {
        automaton.step();
    }
    std::cout << std::string(automaton) << std::endl;
    return 0;
}

#endif
