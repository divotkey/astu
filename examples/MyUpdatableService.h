#pragma once

// AST Utilities includes
#include <AstuServices.h>

class MyUpdatableService 
    : public astu::BaseService
    , private astu::Updatable
{
public:
    
    // Constructor.
    MyUpdatableService(int updatePriority = astu::Priority::Normal);

private:

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

    // Inherited via Updatable
    virtual void OnUpdate () override;
};