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
using VectorIteratorConst = std::vector<unsigned>::const_iterator;
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
              neighborhoodOrder(neighborhoodOrder),
              values(automatonOrder+2*neighborhoodOrder, 0u)
            {
                this->valuesBegin = this->values.begin() + this->neighborhoodOrder;
                this->valuesEnd   = this->values.end()   - this->neighborhoodOrder;
                this->setValues(initialValues);
                return;
            }

        void steps(const unsigned numberOfSteps)
        {
            for(auto i=0u ; i<numberOfSteps ; ++i)
                this->nextStep();
            return;
        }

        const Vector getValues(void) const
        {
            Vector values(this->valuesBegin, this->valuesEnd);
            return values;
        }

    private:
        void nextStep(void)
        {
            Vector newValues(this->automatonOrder, 0u);
            for(unsigned cell=0 ; cell<this->automatonOrder ; ++cell)
            {
                newValues[cell] = this->computeNewValue(cell);
            }
            this->setValues(newValues);
            return;
        }

        unsigned computeNewValue(const unsigned cell) const
        {
            VectorIteratorConst neighborhoodBegin = this->valuesBegin + cell - this->neighborhoodOrder;
            VectorIteratorConst neighborhoodEnd   = this->valuesBegin + cell + this->neighborhoodOrder + 1;
            return std::accumulate(
                    neighborhoodBegin,
                    neighborhoodEnd,
                    0u
                ) % this->cellsOrder;
        }

        unsigned computeDistance(const unsigned lhs, const unsigned rhs) const
        {
            const unsigned differenceClockwise        = absDiff(lhs, rhs);
            const unsigned differenceCounterclockwise = absDiff(this->automatonOrder, differenceClockwise);
            return std::min(differenceClockwise, differenceCounterclockwise);
        }

        void setValues(const Vector & values)
        {
            std::copy(
                values.cbegin(),
                values.cend(),
                this->valuesBegin
            );
            this->copyLeftValues();
            this->copyRightValues();
            return;
        }

        void copyLeftValues(void)
        {
            std::copy(
                this->valuesEnd - this->neighborhoodOrder,
                this->valuesEnd,
                this->values.begin()
            );
            return;
        }

        void copyRightValues(void)
        {
            std::copy(
                this->valuesBegin,
                this->valuesBegin + this->neighborhoodOrder,
                this->values.end() - this->neighborhoodOrder
            );
            return;
        }

    private:
        const unsigned automatonOrder;
        const unsigned cellsOrder;
        const unsigned neighborhoodOrder;
        Vector         values;
        VectorIterator valuesBegin, valuesEnd;
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
