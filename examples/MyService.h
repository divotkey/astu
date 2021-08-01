#pragma once

// AST-Utilities includes
#include <AstuServices.h>

class MyService : public astu::BaseService
{
public:

    // Constructor.
    MyService();

private:

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

};