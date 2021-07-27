// Local includes
#include "MyUpdatableService.h"

using namespace astu;

MyUpdatableService::MyUpdatableService(int updatePriority)
    : BaseService("My Updatable Service")
    , Updatable(updatePriority)
{
    // Intentionally left empty.
}

void MyUpdatableService::OnStartup()
{
    // Place initialization code here...
}

void MyUpdatableService::OnShutdown()
{
    // Place cleanup code here...
}

void MyUpdatableService::OnUpdate()
{
    // Update something.
}
