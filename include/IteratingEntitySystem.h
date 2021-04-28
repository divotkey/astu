// /*
//  * ASTU - AST Utilities
//  * A collection of Utilities for Applied Software Techniques (AST).
//  * 
//  * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
//  */

// #pragma once

// #include <memory>
// #include "ITimeManager.h"
// #include "UpdateService.h"
// #include "EntityService.h"

// namespace astu {

//     class IteratingEntitySystem : public virtual Service, private Updatable, private IEntityListener {
//     public:

//         /**
//          * Constructor.
//          * 
//          * @param iterateFamily     the family of entities this service is processing
//          * @param updatePriority    the priority used to update this service
//          * @param name              the name of this service
//          */
//         IteratingEntitySystem(
//             const EntityFamily & iterateFamily, 
//             int updatePriority = 0, 
//             const std::string & name = "Iterating Entity System");

//         /**
//          * Virtual destructor.
//          */
//         virtual ~IteratingEntitySystem() {}

//     protected:

//         /**
//          * Returns the elapsed time since the last update.
//          * 
//          * @return the delta time in seconds
//          */
//         double GetDeltaTime() const {
//             return timeService->GetElapsedTime();
//         }

//         /**
//          * Returns the entities this system is processing.
//          * 
//          * @return the view to the entities of this system
//          */
//         const EntityView & GetEntityView() const {
//             return *entityView;
//         }

//         /**
//          * Called by this base class for processed entities.
//          * 
//          * @param entity    the current entity to process
//          */
//         virtual void ProcessEntity(astu::Entity & e) = 0;

//         // Inherited via Base Service
//         virtual void Startup() override;
//         virtual void Shutdown() override;

//         // Inherited via IUpdatable
//         virtual void OnUpdate() override;       
//         virtual int GetUpdatePriority() const final override;        

//         // Inherited via IEntityListener
//         virtual void OnEntityAdded(std::shared_ptr<Entity> entity) override {};
//         virtual void OnEntityRemoved(std::shared_ptr<Entity> entity) override {};

//     private:
//         /** The update priority of this updatable. */
//         int updatePriority;

//         /** The family of entities this system is processing. */
//         EntityFamily iterateFamily;

//         /** The view to the entities to be processed. */
//         std::shared_ptr<EntityView> entityView;

//         /** The time service used to offer elapsed delta time. */
//         std::shared_ptr<ITimeManager> timeService;
//     };

// } // end of namespace