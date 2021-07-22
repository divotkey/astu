// /*
//  * ASTU - AST Utilities
//  * A collection of Utilities for Applied Software Techniques (AST).
//  * 
//  * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
//  */

// #include "IteratingEntitySystem.h"

// namespace astu {

//     IteratingEntitySystem::IteratingEntitySystem(const EntityFamily & itFamily, int updatePriority, const std::string & name)
//         : Service(name)
//         , Updatable(updatePriority)
//         , iterateFamily(itFamily)
//     {
//         AddStartupHook([this, updatePriority]{ ASTU_SERVICE(UpdateService).AddUpdatable(this, updatePriority); });
//         AddShutdownHook([this]{  ASTU_SERVICE(UpdateService).RemoveUpdatable(this);  })
//     }

//     void IteratingEntitySystem::Startup() 
//     {
        

//         // Get view to entites.
//         auto & es = ASTU_SERVICE(EntityService);
//         es.AddEntityListener(iterateFamily, shared_as<IteratingEntitySystem>());
//         entityView = es.GetEntityView(iterateFamily);

//         // Get pointer to time service.
//         timeService = ASTU_GET_SERVICE(TimeService);

//         // Don't forget to call Startup on base service!
//         BaseService::Startup();
//     }

//     void IteratingEntitySystem::Shutdown() 
//     {
//         // Don't forget to call shutdown on base service!
//         BaseService::Shutdown();

//         // Cleanup
//         timeService = nullptr;
//         entityView = nullptr;
//         ASTU_SERVICE(EntityService).RemoveEntityListener( iterateFamily, shared_as<IteratingEntitySystem>() );
//         ASTU_SERVICE(UpdateService).RemoveUpdatable( shared_as<IteratingEntitySystem>() );
//     }

//     void IteratingEntitySystem::OnUpdate()
//     {
//         for (auto & entity : *entityView) {
//             ProcessEntity(*entity);
//         }
//     }

//     int IteratingEntitySystem::GetUpdatePriority() const
//     {
//         return updatePriority;
//     }

// } // end of namespace