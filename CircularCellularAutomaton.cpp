#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator>

using VectorIterator = std::vector<unsigned>::iterator;
using Vector = std::vector<unsigned>;

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
            const unsigned automatonOrder,
            const unsigned cellsOrder,
            const unsigned neighborhoodOrder,
            const Vector & initialValues)
            : automatonOrder(automatonOrder),
              cellsOrder(cellsOrder),
              neighborhoodOrder(neighborhoodOrder)
            {
                this->setValues(initialValues);
            }

        void steps(const unsigned numberOfSteps)
        {
            for(auto i=0u ; i<numberOfSteps ; ++i)
                this->nextStep();
            return;
        }

        const Vector & getValues(void) const
        {
            return this->values;
        }

    private:
        void nextStep(void)
        {
            auto newValues = this->values;
            for(unsigned cell=0 ; cell<this->automatonOrder ; ++cell)
            {
                newValues[cell] = this->computeNewValue(cell);
            }
            this->values = newValues;
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

        void setValues(const Vector & values)
        {
            this->values = values;
        }

    private:
        const unsigned automatonOrder;
        const unsigned cellsOrder;
        const unsigned neighborhoodOrder;
        Vector         values;
};

int main()
{
    unsigned automatonOrder = 0;
    unsigned cellsOrder = 0;
    unsigned neighborhoodOrder = 0;
    unsigned numberOfSteps = 0;
    Vector   initialValues{};

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
    const Vector outputValues = automaton.getValues();
    std::copy(outputValues.begin(), outputValues.end(), output_iterator);

}
