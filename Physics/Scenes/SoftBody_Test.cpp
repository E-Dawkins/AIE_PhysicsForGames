#include "SoftBody_Test.h"

#include <string>

void SoftBody_Test::Startup(aie::Application* _app)
{
    SetGravity(glm::vec2(0, -10));
    
    std::vector<std::string> E;
    E.emplace_back("00000");
    E.emplace_back("00---");
    E.emplace_back("00000");
    E.emplace_back("00---");
    E.emplace_back("00000");

    std::vector<std::string> T;
    T.emplace_back("00000");
    T.emplace_back("--0--");
    T.emplace_back("--0--");
    T.emplace_back("--0--");
    T.emplace_back("--0--");

    std::vector<std::string> H;
    H.emplace_back("0---0");
    H.emplace_back("0---0");
    H.emplace_back("00000");
    H.emplace_back("0---0");
    H.emplace_back("0---0");

    std::vector<std::string> A;
    A.emplace_back("-000-");
    A.emplace_back("0---0");
    A.emplace_back("00000");
    A.emplace_back("0---0");
    A.emplace_back("0---0");

    std::vector<std::string> N;
    N.emplace_back("0---0");
    N.emplace_back("00--0");
    N.emplace_back("0-0-0");
    N.emplace_back("0--00");
    N.emplace_back("0---0");

    SoftBody::Build(this, glm::vec2(-85, 0), 10.f, 30.f, 1.5f, E);
    SoftBody::Build(this, glm::vec2(-50, 0), 10.f, 30.f, 1.5f, T);
    SoftBody::Build(this, glm::vec2(-15, 0), 10.f, 30.f, 1.5f, H);
    SoftBody::Build(this, glm::vec2(20, 0), 10.f, 30.f, 1.5f, A);
    SoftBody::Build(this, glm::vec2(60, 0), 10.f, 30.f, 1.5f, N);

    Plane* plane = new Plane(glm::vec2(0, 1), -50);
    AddActor(plane);
}
