#ifndef IK_SOLVER_H
#define IK_SOLVER_H

#include <vector>


std::vector<float> solveIK(const std::vector<std::vector<float>>& dhTable, float x, float y, float z);

#endif // IK_SOLVER_H