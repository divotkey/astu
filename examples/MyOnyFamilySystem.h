#pragma once


// AST Utilities includes
#include <AstuECS.h>

class MyOnyFamilySystem 
    : public astu::BaseService
    , private astu::OneFamilyEntitySystem
{
public:

    // Constructor.
    MyOnyFamilySystem();

private:
    // The family of entities this system processes
    static const astu::EntityFamily FAMILY;

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
};