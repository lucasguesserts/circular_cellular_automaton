#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>

std::string to_string(std::vector<unsigned> vec)
{
    std::string str = "";
    for(auto it=vec.cbegin() ; it<vec.cend()-1 ; ++it)
    {
        str += std::to_string(*it) + std::string(" ");
    }
    str += std::to_string(vec.back());
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
              environmentDistance(environmentDistance),
              values(initialValues)
            {}

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

        unsigned computeNewValue(const unsigned cell) const
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

        unsigned computeDistance(const unsigned lhs, const unsigned rhs) const
        {
            const unsigned difference = lhs > rhs ? lhs-rhs : rhs-lhs;
            const unsigned differenceWithOrder = this->automatonOrder > difference \
                ? this->automatonOrder - difference \
                : difference - this->automatonOrder ;
            return difference > differenceWithOrder ? differenceWithOrder : difference;
        }

        operator std::string() const
        {
            return to_string(this->values);
        }

    private:
        const unsigned        automatonOrder;
        const unsigned        cellsOrder;
        const unsigned        environmentDistance;
        std::vector<unsigned> values;
};

int main()
{
    unsigned              automatonOrder = 0;
    unsigned              cellsOrder = 0;
    unsigned              environmentDistance = 0;
    unsigned              numberOfSteps = 0;
    std::vector<unsigned> initialValues{};

    std::cin >> automatonOrder >> cellsOrder >> environmentDistance >> numberOfSteps;
    initialValues.resize(automatonOrder);
    for(unsigned i=0 ; i<automatonOrder ; ++i)
    {
        std::cin >> initialValues[i];
    }

    CircularCellularAutomaton automaton(
        automatonOrder,
        cellsOrder,
        environmentDistance,
        initialValues
    );
    automaton.steps(numberOfSteps);
    std::cout << std::string(automaton);

    return 0;
}
