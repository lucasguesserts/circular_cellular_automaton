#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>

unsigned absDiff(const unsigned lhs, const unsigned rhs)
{
    return lhs > rhs \
           ? lhs - rhs \
           : rhs - lhs;
}

class CircularCellularAutomaton
{
    public:
        CircularCellularAutomaton(
            const unsigned              automatonOrder,
            const unsigned              cellsOrder,
            const unsigned              neighborhoodOrder,
            const std::vector<unsigned> initialValues)
            : automatonOrder(automatonOrder),
              cellsOrder(cellsOrder),
              neighborhoodOrder(neighborhoodOrder),
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
                if (this->computeDistance(cell, otherCell) <= this->neighborhoodOrder)
                {
                    accumulator += this->values[otherCell];
                }
            }
            return accumulator % this->cellsOrder;
        }

        unsigned computeDistance(const unsigned lhs, const unsigned rhs) const
        {
            const unsigned differenceClockwise        = absDiff(lhs, rhs);
            const unsigned differenceCounterclockwise = absDiff(this->automatonOrder, differenceClockwise);
            return std::min(differenceClockwise, differenceCounterclockwise);
        }

    public:
        const unsigned        automatonOrder;
        const unsigned        cellsOrder;
        const unsigned        neighborhoodOrder;
        std::vector<unsigned> values;
};

int main()
{
    unsigned              automatonOrder = 0;
    unsigned              cellsOrder = 0;
    unsigned              neighborhoodOrder = 0;
    unsigned              numberOfSteps = 0;
    std::vector<unsigned> initialValues{};

    std::fstream inputFile;
    inputFile.open("cell.in", std::ios::in);

    {
        std::string line;
        std::getline(inputFile, line);
        std::stringstream ss(line);
        ss >> automatonOrder >> cellsOrder >> neighborhoodOrder >> numberOfSteps;
    }

    {
        initialValues.resize(automatonOrder);
        std::string line;
        std::getline(inputFile, line);
        std::stringstream ss(line);
        for(unsigned i=0 ; i<automatonOrder ; ++i)
        {
            ss >> initialValues[i];
        }
    }

    CircularCellularAutomaton automaton(
        automatonOrder,
        cellsOrder,
        neighborhoodOrder,
        initialValues
    );
    automaton.steps(numberOfSteps);

    std::fstream outputFile;
    outputFile.open("cell.out", std::ios::out);
    std::ostream_iterator<unsigned> output_iterator(outputFile, " ");
    std::copy(automaton.values.begin(), automaton.values.end(), output_iterator);

}
