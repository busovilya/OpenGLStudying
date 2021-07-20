#pragma once

#include <iostream>

#include <glm/glm.hpp>

std::ostream& operator<<(std::ostream& out, const glm::mat4& mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            out << mat[i][j] << ' ';
        }
        out << std::endl;
    }

    return out;
}