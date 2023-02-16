#include "SoftBody.h"
#include "Circle.h"
#include "PhysicsScene.h"

void SoftBody::Build(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
    int numColumns = (int)_strings.size();
    int numRows = (int)_strings[0].length();
    
    // Traverse across the array and add balls where the ascii art says they should be
    Circle** circles = new Circle*[numRows * numColumns];
    
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (_strings[j][i] == '0')
            {
                float circleRadius = 2.f;
                glm::vec2 circlePos = _position + glm::vec2(i, -j) * (2 * circleRadius + _spacing);
                
                circles[i * numColumns + j] = new Circle(circlePos, glm::vec2(0), 1.f, circleRadius);
                _scene->AddActor(circles[i * numColumns + j]);
            }
            else circles[i * numColumns + j] = nullptr;
        }
    }
    
    // Second pass, add springs in
    for (int i = 1; i < numRows; i++)
    {
        for (int j = 1; j < numColumns; j++)
        {
            Circle* s11 = circles[i * numColumns + j];
            Circle* s01 = circles[(i - 1) * numColumns + j];
            Circle* s10 = circles[i * numColumns + (j - 1)];
            Circle* s00 = circles[(i - 1) * numColumns + (j - 1)];
            
            // Make spring to cardinal neighbours
            if (s11 && s01)
                _scene->AddActor(new Spring(s11, s01, _damping, _springForce));

            if (s11 && s10)
                _scene->AddActor(new Spring(s11, s10, _damping, _springForce));

            if (s10 && s00)
                _scene->AddActor(new Spring(s10, s00, _damping, _springForce));

            if (s01 && s00)
                _scene->AddActor(new Spring(s01, s00, _damping, _springForce));

            // Diagonal springs
            if (s11 && s00)
                _scene->AddActor(new Spring(s11, s00, _damping, _springForce));

            if (s01 && s10)
                _scene->AddActor(new Spring(s01, s10, _damping, _springForce));
            
            // Bend springs
            bool endOfJ = j == numColumns - 1;
            bool endOfI = i == numRows - 1;
            
            Circle* s22 = (!endOfI && !endOfJ) ? circles[(i + 1) * numColumns + (j + 1)] : nullptr;
            Circle* s02 = !endOfJ ? circles[(i - 1) * numColumns + (j + 1)] : nullptr;
            Circle* s20 = !endOfI ? circles[(i + 1) * numColumns + (j - 1)] : nullptr;

            // Another set of cardinal springs
            if (s22 && s02)
                _scene->AddActor(new Spring(s22, s02, _damping, _springForce));
            
            if (s22 && s20)
                _scene->AddActor(new Spring(s22, s20, _damping, _springForce));
            
            if (s20 && s00)
                _scene->AddActor(new Spring(s20, s00, _damping, _springForce));
            
            if (s02 && s00)
                _scene->AddActor(new Spring(s02, s00, _damping, _springForce));
        }
    }
}
