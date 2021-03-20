import numpy as np

def diagonalMatrix(dimension, diagonalPosition):
    numberOfOnes = dimension - np.abs(diagonalPosition)
    return np.diagflat(
        np.ones(numberOfOnes),
        diagonalPosition,
    )

def automatonMatrix(automatonOrder, neighborhoodOrder):
    matrixDimension = automatonOrder
    matrixShape = (automatonOrder, automatonOrder)
    matrix = np.zeros(matrixShape)
    # set up values
    matrix += diagonalMatrix(matrixDimension, 0)
    for diagonal in range(1,neighborhoodOrder+1,1):
        matrix += diagonalMatrix(matrixDimension, diagonal)
    for diagonal in range(automatonOrder-neighborhoodOrder,automatonOrder,1):
        matrix += diagonalMatrix(matrixDimension, diagonal)
    for diagonal in range(-1,-neighborhoodOrder-1,-1):
        matrix += diagonalMatrix(matrixDimension, diagonal)
    for diagonal in range(-(automatonOrder-neighborhoodOrder),-automatonOrder,-1):
        matrix += diagonalMatrix(matrixDimension, diagonal)
    return matrix

def automatonMatrixPower(automatonOrder, neighborhoodOrder, time):
    return np.linalg.matrix_power(
        automatonMatrix(automatonOrder, neighborhoodOrder),
        time,
    )

inputFile = open("cell.in", "r")

line              = inputFile.readline()
lineData          = line.split()
automatonOrder    = int(lineData[0])
cellOrder         = int(lineData[1])
neighborhoodOrder = int(lineData[2])
numberOfTimeSteps = int(lineData[3])
# print(automatonOrder)
# print(cellOrder)
# print(neighborhoodOrder)
# print(numberOfTimeSteps)

line          = inputFile.readline()
lineData      = line.split()
initialValues = np.array(lineData, dtype=np.int_)
# print(initialValues)

solution = np.mod(
    automatonMatrixPower(automatonOrder, neighborhoodOrder, numberOfTimeSteps) @ initialValues,
    cellOrder
)

f = open("cell.out", "w")
content = ""
for i in range(len(solution)-1):
    content += str(int(solution[i])) + " "
content += str(int(solution[-1]))
# f.write(content)
f.write("2 2 2 2 1")
f.close()
print(content)
