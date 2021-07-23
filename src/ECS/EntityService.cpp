/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "ECS/EntityService.h"

// C++ Standard Library includes.
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace astu {
    
    /////////////////////////////////////////////////
    /////// Entity
    /////////////////////////////////////////////////

    void Entity::AddComponent(shared_ptr<EntityComponent> cmp)
    {
        // Workaround to silence warning on macOS. Without macOS compilers
        // the code should look like this:
        // auto type = type_index(typeid(*cmp));
        auto & c = *cmp;
        auto type = type_index(typeid(c));

        // Ensure component is the only one of its type.
        if (HasComponent(type)) {
            throw logic_error(string("Component of type ' ") 
                + type.name() + "' already added to entity");
        }

        // Add component to component list.
        assert(find(components.begin(), components.end(), cmp) == components.end());
        components.push_back(cmp);

        // Add component type to map for fast access.
        assert(compMap.find(type) == compMap.end());
        compMap[type] = cmp;

        // Assing this entity as parent.
        cmp->parent = shared_from_this();

        // Inform component that it has been added.
        cmp->OnAddedToEntity(*this);

    }

    void Entity::AddInterface(EntityComponent& cmp, const type_index &type)
    {
		if (!HasComponent(typeid(cmp))) {
			throw logic_error(
                string("Unable to add interface, unknown component  '") 
				+ typeid(cmp).name() + "'");
		}
        
		if (compMap.find(type) != compMap.end()) {
			throw logic_error(
                string("Unable to add interface, already added: '") 
				+ type.name() + "'");
		}

		compMap[type] = compMap[typeid(cmp)];
    }

    bool Entity::HasComponent(const type_index &type) const
    {
        return compMap.find(type) != compMap.end();
    }

    EntityComponent &Entity::GetComponent(const type_index &type)
    {
        auto it = compMap.find(type);
        if (it == compMap.end())
        {
            throw logic_error(
                string("Unknown component type '") + type.name() + "'");
        }

        return *it->second;
    }

    const EntityComponent &Entity::GetComponent(const type_index &type) const
    {
        auto it = compMap.find(type);
        if (it == compMap.end())
        {
            throw logic_error(
                string("Unknown component type '") + type.name() + "'");
        }

        return *it->second;
    }

    /////////////////////////////////////////////////
    /////// EntityService
    /////////////////////////////////////////////////

    EntityService::EntityService(int updatePriority)
        : Service("Entity Service")
        , Updatable(updatePriority)
    {
        // Intentionally left empty.
    }

    const shared_ptr<EntityView> EntityService::GetEntityView(const EntityFamily &family)
    {
        const auto &it = viewMap.find(family);
        if (it != viewMap.end())
        {
            // Family of entities does already exist.
            return it->second;
        }

        // Create new view and add associated entities.
        auto view = make_shared<EntityView>();
        viewMap[family] = view;
        for (const auto &entity : entities)
        {
            if (family.IsMember(*entity))
            {
                view->push_back(entity);
            }
        }

        return view;
    }

    void EntityService::AddEntity(shared_ptr<Entity> entity)
    {
        commands.Add([this, entity](){ AddEntityInternally(entity); });
    }

    void EntityService::RemoveEntity(shared_ptr<Entity> entity)
    {
        commands.Add([this, entity](){ RemoveEntityInternally(entity); });
    }

    bool EntityService::HasEntity(shared_ptr<Entity> entity) const
    {
        return find(entities.begin(), entities.end(), entity) != entities.end();
    }

    void EntityService::RemoveAll()
    {
        commands.Add([this](){ RemoveAllInternally(); });
    }    

    void EntityService::OnStartup()
    {
        firing = false;
        idCounter = 0;
    }

    void EntityService::OnShutdown()
    {
        RemoveAllInternally();
    }

    void EntityService::OnUpdate()
    {
        commands.Execute();
    }

    void EntityService::AddEntityInternally(shared_ptr<Entity> entity)
    {
		// Add entity to entity families.
		for (auto & it : viewMap) {
			if (it.first.IsMember(*entity)) {
				it.second->push_back(entity);
			}
		}

		// Add entity.
		entities.push_back(entity);

        // Assign unique entity ID.
        entity->id = ++idCounter;

        // Inform entity listeners.
        firing = true;
		for (auto it : listeners) {
			if (it.first.IsMember(*entity)) {
                FireEntityAdded(it.second, entity);
			}
		}
        firing = false;
    }

    void EntityService::RemoveEntityInternally(shared_ptr<Entity> entity)
    {
        if (!HasEntity(entity)) {
            return;
        }

        // Fire entity removed event.
        firing = true;
		for (auto it : listeners) {
			if (it.first.IsMember(*entity)) {
                FireEntityRemoved(it.second, entity);
			}
		}
        firing = false;

		// Remove entity from entity views.
		for (auto & it : viewMap) {
			RemoveFromView(*it.second, entity);
		}

        // Remove entity.
		entities.erase(
            remove(entities.begin(), entities.end(), entity), 
            entities.end());
    }

    void EntityService::RemoveFromView(EntityView & view, shared_ptr<Entity> entity)
    {
		view.erase(remove(view.begin(), view.end(), entity), view.end());
    }

    void EntityService::RemoveAllInternally()
    {
		while (!entities.empty()) {
			RemoveEntityInternally(entities.back());
		}
    }

    bool EntityService::HasEntityListener(const EntityFamily & family, IEntityListener &  listener) const
    {
        auto it = listeners.find(family);
        if (it == listeners.end()) {
            return false;
        }

        const ListenerList & familyListeners = it->second;
        return find(familyListeners.begin(), familyListeners.end(), &listener) != familyListeners.end();
    }

    void EntityService::AddEntityListener(const EntityFamily & family, IEntityListener & listener)
    {
		if (firing) {
			throw logic_error("Entity listeners must not be added while firing entity events");
		}

        if (HasEntityListener(family, listener)) {
            throw logic_error("Entity listener already added");
        }

        listeners[family].push_back(&listener);        
    }

    void EntityService::RemoveEntityListener(const EntityFamily & family, IEntityListener & listener)
    {
		if (firing) {
			throw logic_error("Entity listeners must not be removed while firing entity events");
		}

        auto it = listeners.find(family);
        if (it == listeners.end()) {
            return;
        }

        for (auto listIt = it->second.begin(); listIt != it->second.end(); ) {
            if (*listIt == &listener) {
                listIt = it->second.erase(listIt);
            } else {
                ++listIt;
            }
        }
    }

    void EntityService::FireEntityAdded(ListenerList & lst, shared_ptr<Entity> e)
    {
        for (auto listener : lst) {
            listener->OnEntityAdded(e);
        }
    }

    void EntityService::FireEntityRemoved(ListenerList & lst, shared_ptr<Entity> e)
    {
        for (auto listener : lst) {
            listener->OnEntityRemoved(e);
        }
    }

} // namespace astu