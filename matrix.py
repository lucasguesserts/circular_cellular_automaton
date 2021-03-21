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

automatonOrder = 5
neighborhoodOrder = 1
cellOrder = 3
numberOfTimeSteps = 10
initialState = np.array([
    1, 2, 2, 1, 2
])

matrix = automatonMatrixPower(
    automatonOrder,
    neighborhoodOrder,
    numberOfTimeSteps
)

solution = np.mod(
    matrix @ initialState,
    cellOrder
)

print(solution)
