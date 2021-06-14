/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <string>
#include <stdexcept>
#include <algorithm>
#include "EntityService.h"

namespace astu {
    
    /////////////////////////////////////////////////
    /////// Entity
    /////////////////////////////////////////////////

    void Entity::AddComponent(std::shared_ptr<EntityComponent> cmp)
    {

        // working with reference to silence warning on macOS.
        auto & c = *cmp;
        auto type = std::type_index(typeid(c));

        // Original code.
        // auto type = std::type_index(typeid(*cmp));

        if (compMap.find(type) != compMap.end())
        {
            throw std::logic_error(std::string("Component of type ' ") + type.name() + "' already added to entity");
        }

        compMap[type] = cmp;
    }

    bool Entity::HasComponent(const std::type_index &type) const
    {
        return compMap.find(type) != compMap.end();
        ;
    }

    EntityComponent &Entity::GetComponent(const std::type_index &type)
    {
        auto it = compMap.find(type);
        if (it == compMap.end())
        {
            throw std::logic_error(
                std::string("Unknown component type '") + type.name() + "'");
        }

        return *it->second;
    }

    const EntityComponent &Entity::GetComponent(const std::type_index &type) const
    {
        auto it = compMap.find(type);
        if (it == compMap.end())
        {
            throw std::logic_error(
                std::string("Unknown component type '") + type.name() + "'");
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

    const std::shared_ptr<EntityView> EntityService::GetEntityView(const EntityFamily &family)
    {
        const auto &it = viewMap.find(family);
        if (it != viewMap.end())
        {
            // Family of entities does already exist.
            return it->second;
        }

        // Create new view and add associated entities.
        auto view = std::make_shared<EntityView>();
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

    void EntityService::AddEntity(std::shared_ptr<Entity> entity)
    {
        commands.Add([this, entity](){ AddEntityInternally(entity); });
    }

    void EntityService::RemoveEntity(std::shared_ptr<Entity> entity)
    {
        commands.Add([this, entity](){ RemoveEntityInternally(entity); });
    }

    void EntityService::RemoveAll()
    {
        commands.Add([this](){ RemoveAllInternally(); });
    }    

    void EntityService::OnStartup()
    {
        firing = false;
    }

    void EntityService::OnShutdown()
    {
        RemoveAllInternally();
    }

    void EntityService::OnUpdate()
    {
        commands.Execute();
    }

    void EntityService::AddEntityInternally(std::shared_ptr<Entity> entity)
    {
		// Add entity to entity families.
		for (auto & it : viewMap) {
			if (it.first.IsMember(*entity)) {
				it.second->push_back(entity);
			}
		}

		// Add entity.
		entities.push_back(entity);

        firing = true;
		for (auto it : listeners) {
			if (it.first.IsMember(*entity)) {
                FireEntityAdded(it.second, entity);
			}
		}
        firing = false;
    }

    void EntityService::RemoveEntityInternally(std::shared_ptr<Entity> entity)
    {
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
            std::remove(entities.begin(), entities.end(), entity), 
            entities.end());
    }

    void EntityService::RemoveFromView(EntityView & view, std::shared_ptr<Entity> entity)
    {
		view.erase(std::remove(view.begin(), view.end(), entity), view.end());
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
        return std::find(familyListeners.begin(), familyListeners.end(), &listener) != familyListeners.end();
    }

    void EntityService::AddEntityListener(const EntityFamily & family, IEntityListener & listener)
    {
		if (firing) {
			throw std::logic_error("Entity listeners must not be added while firing entity events");
		}

        if (HasEntityListener(family, listener)) {
            throw std::logic_error("Entity listener already added");
        }

        listeners[family].push_back(&listener);        
    }

    void EntityService::RemoveEntityListener(const EntityFamily & family, IEntityListener & listener)
    {
		if (firing) {
			throw std::logic_error("Entity listeners must not be removed while firing entity events");
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

    void EntityService::FireEntityAdded(ListenerList & listeners, std::shared_ptr<Entity> entity)
    {
        for (auto listener : listeners) {
            listener->OnEntityAdded(entity);
        }
    }

    void EntityService::FireEntityRemoved(ListenerList & listeners, std::shared_ptr<Entity> entity)
    {
        for (auto listener : listeners) {
            listener->OnEntityRemoved(entity);
        }
    }

} // namespace astu