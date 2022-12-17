#include "ship.h"

Ship::Ship()
{

}

Ship::Ship(Mesh* mesh, Camera* camera)
{
    m_mesh = mesh;
    m_camera = camera;
}

Ship::~Ship()
{

}

void Ship::Update(double deltaTime)
{
    
}

// void Ship::SetVelocity