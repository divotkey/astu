#include "MyOnyFamilySystem.h"

using namespace astu;
using namespace std;

// TODO add types of entity components to the family e.g.,
// EntityFamily::Create<astu2d::CPose, astu2d::CBody>();

const EntityFamily MyOnyFamilySystem::FAMILY = EntityFamily::Create<>();

MyOnyFamilySystem::MyOnyFamilySystem(int updatePriority)
    : BaseService("My Ony-Family System")
    , Updatable(updatePriority)
    , OneFamilyEntitySystem(FAMILY)    
{
    // Intentionally left empty.
}

void MyOnyFamilySystem::OnStartup()
{
    // Intentionally left empty.
}

void MyOnyFamilySystem::OnShutdown()
{
    // Intentionally left empty.
}

void MyOnyFamilySystem::OnUpdate()
{
    ProcessEntities();
} 

void MyOnyFamilySystem::ProcessEntity(Entity & entity)
{
    // Process entity    
}