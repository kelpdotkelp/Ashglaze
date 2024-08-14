#include "coordinateAxis.h"

#include <glad/glad.h>

#include "../shaderProgram.h"
#include "../num/num.h"

namespace world
{
    CoordinateAxis::CoordinateAxis()
    {

    }

    void CoordinateAxis::setup()
    {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBufferData(GL_ARRAY_BUFFER, VBOData.size()*sizeof(float), VBOData.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOData.size()*sizeof(int), EBOData.data(), GL_STATIC_DRAW);
    }

    void CoordinateAxis::render(ShaderProgram& shaderProgram, num::Mat4 view, num::Mat4 projection)
    {
        num::Mat4 model = num::Mat4();

        glBindVertexArray(VAO);
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);

        //Y
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec3("color", 0.0f, 1.0f, 0.0f);
        glDrawElements(GL_TRIANGLES, EBOData.size(), GL_UNSIGNED_INT, 0);

        //Z
        model = num::rotateX(model, 90.0f);
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec3("color", 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, EBOData.size(), GL_UNSIGNED_INT, 0);

        //X
        model = num::rotateY(model, 90.0f);
        shaderProgram.setMat4("model", model);
        shaderProgram.setVec3("color", 1.0f, 0.0f, 0.0f);
        glDrawElements(GL_TRIANGLES, EBOData.size(), GL_UNSIGNED_INT, 0);
    }

    float arrowShaftLength = 0.01f;
    float arrowHeadLength = 0.02f;
    float arrowHeadHeight = 0.1f;
    std::vector<float> CoordinateAxis::VBOData{
        //Create a single arrow, rotate later for other 2 axis
        //Arrow shaft
        /*00*/-arrowShaftLength, 0.0f, -arrowShaftLength,
        /*01*/ arrowShaftLength, 0.0f, -arrowShaftLength,
        /*02*/ arrowShaftLength, 0.0f,  arrowShaftLength,
        /*03*/-arrowShaftLength, 0.0f,  arrowShaftLength,
        /*04*/-arrowShaftLength, 1.0f - arrowHeadHeight, -arrowShaftLength,
        /*05*/ arrowShaftLength, 1.0f - arrowHeadHeight, -arrowShaftLength,
        /*06*/ arrowShaftLength, 1.0f - arrowHeadHeight,  arrowShaftLength,
        /*07*/-arrowShaftLength, 1.0f - arrowHeadHeight,  arrowShaftLength,
        //Arrow head (tetrahedron)
        /*08*/-arrowHeadLength, 1.0f - arrowHeadHeight, -arrowHeadLength,
        /*09*/ arrowHeadLength, 1.0f - arrowHeadHeight, -arrowHeadLength,
        /*10*/ arrowHeadLength, 1.0f - arrowHeadHeight,  arrowHeadLength,
        /*11*/-arrowHeadLength, 1.0f - arrowHeadHeight,  arrowHeadLength,
        /*12*/0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    };

    std::vector<int> CoordinateAxis::EBOData{
        //Arrow shaft
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        0, 1, 5,
        0, 5, 4,
        1, 2, 6,
        1, 5, 6,
        2, 6, 7,
        2, 3, 7,
        3, 0, 4,
        3, 4, 7,
        //Arrow head
        8,  9,  10,
        8,  10, 11,
        8,  9,  12,
        9,  10, 12,
        10, 11, 12,
        11, 8,  12,
    };
}