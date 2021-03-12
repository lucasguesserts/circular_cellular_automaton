#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>

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

    public:
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

    std::fstream inputFile;
    inputFile.open("cell.in", std::ios::in);

    {
        std::string line;
        std::getline(inputFile, line);
        std::stringstream ss(line);
        ss >> automatonOrder >> cellsOrder >> environmentDistance >> numberOfSteps;
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
        environmentDistance,
        initialValues
    );
    automaton.steps(numberOfSteps);

    std::fstream outputFile;
    outputFile.open("cell.out", std::ios::out);
    std::ostream_iterator<unsigned> output_iterator(outputFile, " ");
    std::copy(automaton.values.begin(), automaton.values.end(), output_iterator);

}
