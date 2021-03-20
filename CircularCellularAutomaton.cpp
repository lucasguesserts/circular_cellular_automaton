#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>

using Vector              = std::vector<unsigned>;
using VectorIterator      = Vector::iterator;
using VectorIteratorConst = Vector::const_iterator;

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
            unsigned currentValue = this->computeInitialValue();
            newValues[0u] = currentValue % this->cellsOrder;
            for(unsigned cell=1u ; cell<this->automatonOrder ; ++cell)
            {
                currentValue = this->computeNextValue(currentValue, cell);
                newValues[cell] = currentValue % this->cellsOrder;
            }
            this->setValues(newValues);
            return;
        }

        unsigned computeInitialValue() const
        {
            constexpr unsigned cell = 0u;
            VectorIteratorConst neighborhoodBegin = this->valuesBegin + cell - this->neighborhoodOrder;
            VectorIteratorConst neighborhoodEnd   = this->valuesBegin + cell + this->neighborhoodOrder + 1u;
            return std::accumulate(
                neighborhoodBegin,
                neighborhoodEnd,
                0u
            );
        }

        unsigned computeNextValue(const unsigned currentValue, const unsigned cell) const
        {
            VectorIteratorConst previousFirstValue = this->valuesBegin + cell - this->neighborhoodOrder - 1u;
            VectorIteratorConst currentLastValue   = this->valuesBegin + cell + this->neighborhoodOrder;
            return currentValue
                - *previousFirstValue
                + *currentLastValue;
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
    unsigned automatonOrder    = 0u;
    unsigned cellsOrder        = 0u;
    unsigned neighborhoodOrder = 0u;
    unsigned numberOfSteps     = 0u;
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
