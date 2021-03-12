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

        void steps(const unsigned numberOfSteps)
        {
            for(auto i=0u ; i<numberOfSteps ; ++i)
                this->step();
            return;
        }

        unsigned computeNewValue(const unsigned cell)
        {
            unsigned accumulator = 0;
            for(unsigned otherCell=0 ; otherCell<this->automatonOrder ; ++otherCell)
            {
                if (this->computeDistance(cell, otherCell) <= this->environmentDistance)
                {
                    accumulator += this->values[otherCell];
                }
            }
            return accumulator % this->cellsOrder;
        }

        unsigned computeDistance(const unsigned lhs, const unsigned rhs)
        {
            const unsigned difference = lhs > rhs ? lhs-rhs : rhs-lhs;
            const unsigned differenceWithOrder = this->automatonOrder > difference \
                ? this->automatonOrder - difference \
                : difference - this->automatonOrder ;
            return difference > differenceWithOrder ? differenceWithOrder : difference;
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
                   std::string("---------------------------")                                         ;
            return str;
        }

    private:
        const unsigned        automatonOrder;
        const unsigned        cellsOrder;
        const unsigned        environmentDistance;
        std::vector<unsigned> values;
};

void test_case_0(void)
{
    const unsigned numberOfSteps = 1;
    CircularCellularAutomaton automaton(5, 3, 1, {1, 2, 2, 1, 2});
    automaton.steps(numberOfSteps);
    std::cout << std::string(automaton) << std::endl;
    std::cout << "expected result: " + to_string(std::vector<unsigned>{2, 2, 2, 2, 1}) << std::endl;
    std::cout << std::endl;
}

void test_case_1(void)
{
    const unsigned numberOfSteps = 10;
    CircularCellularAutomaton automaton(5, 3, 1, {1, 2, 2, 1, 2});
    automaton.steps(numberOfSteps);
    std::cout << std::string(automaton) << std::endl;
    std::cout << "expected result: " + to_string(std::vector<unsigned>{2, 0, 0, 2, 2}) << std::endl;
    std::cout << std::endl;
}

int main()
{
    unsigned              automatonOrder = 0;
    unsigned              cellsOrder = 0;
    unsigned              environmentDistance = 0;
    unsigned              numberOfSteps = 0;
    std::vector<unsigned> initialValues{};

    std::cout << "Insert (n, m, d, k): ";
    std::cin >> automatonOrder >> cellsOrder >> environmentDistance >> numberOfSteps;
    std::cout << "Insert " << std::to_string(automatonOrder) << \
        " initial values separated by spaces: ";
    for(unsigned i=0 ; i<automatonOrder ; ++i)
    {
        unsigned value;
        std::cin >> value;
        initialValues.push_back(value);
    }

    CircularCellularAutomaton automaton(
        automatonOrder,
        cellsOrder,
        environmentDistance,
        initialValues
    );
    automaton.steps(numberOfSteps);
    std::cout << std::string(automaton) << std::endl;
    return 0;
}

#endif
