#pragma once


// AST Utilities includes
#include <AstuECS.h>

class MyOnyFamilySystem 
    : public astu::BaseService
    , private astu::OneFamilyEntitySystem
    , private astu::Updatable
{
public:

    // Constructor.
    MyOnyFamilySystem(int updatePriority = astu::Priority::Normal);

private:
    // The family of entities this system processes
    static const astu::EntityFamily FAMILY;

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

    // Inherited via Updatable
    virtual void OnUpdate() override;

    // Inherited via OneFamilyEntitySystem
    virtual void ProcessEntity(astu::Entity & entity) override;
};