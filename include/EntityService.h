/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <set>

// Local includes.
#include "UpdateService.h"
#include "CommandQueue.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// EntityComponent
    /////////////////////////////////////////////////

    /**
     * Base class for all entity components.
     * 
     * @ingroup ecs_group
     */
    class EntityComponent {
    public:

        /**
         * Constructor.
         */
        EntityComponent() = default;

        /**
         * Virtual destructor.
         */
        virtual ~EntityComponent() {}

        /**
         * Creates a copy of this entity component.
         * 
         * @return a copy of this componend
         */
        virtual std::shared_ptr<EntityComponent> Clone() = 0;
    };


    /////////////////////////////////////////////////
    /////// Entity
    /////////////////////////////////////////////////

    /**
     * An entity is a container for compoments. 
     * 
     * @ingroup ecs_group
     */
    class Entity final : public std::enable_shared_from_this<Entity> {
    public:

        /**
         * Constructor.
         */
        Entity() {
            // Intentionally left empty.            
        }

        /**
         * Adds a component to this entity.
         * 
         * @param cmp   the component to add
         */
        void AddComponent(std::shared_ptr<EntityComponent> cmp);
        
		/**
		 * Tests if specified type of component has been added to this entity.
		 *
		 * This method is mainly used by the template method `HasComponent`,
         * which is more convenient to use.
         * 
		 * This method requires a std::type_index parameter to specify the type of
		 * component to be tested. The C++ `typeid` operator can be used to get
		 * the std::type_info object, which will be automatically converted
		 * to a std::type_index object when calling this method.
		 *
		 * **Usage example:**
		 * ```cpp
		 * if ( entity.hasComponent( typeid(Pose2D) ) ) {
		 * 	// do something with Pose2D component
		 * }
		 * ```
		 *
		 * @param type a type_index specifying the type of component to test
		 * @return `true` if a component of the specified type exists
		 */
        bool HasComponent(const std::type_index & type) const;

		/**
		 * Returns the component of a specific type.
		 *
		 * This method is mainly used by the template method `GetComponent`,
         * which is much more convenient to use because no type cast is
         * required.
		 *
		 * A dynamic cast is not necessary because this method ensures to return
		 * a component of the required type which can safely be casted using
		 * a fast static cast.
		 *
		 * **Usage example:**
		 * ```cpp
		 * auto & pose = static_cast<Pose2D &>( entity.GetComponent( std::type_index( typeid(Pose2D) ) ) );
		 * ```
		 *
		 * @param type the component type
		 * @return the requested component
		 * @throws std::logic_error in case the requested component does not exist
		 */
        EntityComponent& GetComponent(const std::type_index & type);

		/**
		 * Returns the component of a specific type.
         * 
		 * This is the constant version of this method. See the non-constant
		 * version description for details.

		 * @param type the component type
		 * @return the requested component
		 * @throws std::logic_error in case the requested component does not exist
		 */
		const EntityComponent& GetComponent(const std::type_index & type) const;

		/**
		 * Retrieves the component a specific type from this entity.
		 *
		 * This template method offers a convenient way to retrieve a component by
		 * specifying the type of the component to retrieve as template parameter.
		 *
		 * **Usage example:**
		 * ```cpp
		 * auto & pose = entity.GetComponent<Pose2D>();
		 * ```
		 *
		 * @tparam T	the type of the component to retrieve
		 * @return the requested component
		 * @throws std::logic_error in case the requested component does not exist
		 */
		template<typename T> T& GetComponent() {
			return static_cast<T&>( GetComponent( typeid(T) ) );
		}

		/**
		 * Retrieves the component a specific type from this entity.
		 *
		 * This template method offers a convenient way to retrieve a component by
		 * specifying the type of the component to retrieve as template parameter.
		 *
		 * **Usage example:**
		 * ```cpp
		 * const auto & pose = entity.GetComponent<Pose2D>();
		 * ```
		 *
		 * @tparam T	the type of the component to retrieve
		 * @return the requested component
		 * @throws std::logic_error in case the requested component does not exist
		 */
        template<typename T> const T& GetComponent() const {
			return static_cast<const T&>( GetComponent( typeid(T) ) );
		}

		/**
		 * Tests whether a component of a specific type has been added to this entity.
		 *
		 * This template method offers a convenient way to test if a certain component type
		 * exists in this entity by specifying the type of the component as template
		 * parameter.
		 *
		 * **Usage example:**
		 * ```cpp
		 * if ( entity.hasComponent<Pose2D>() ) {
		 * 	// do something with Pose2D component
		 * }
		 * ```
		 *
		 * @tparam T	the type of component to be tested
		 * @return `true` if a component of the specified type exists
		 */
		template<typename T> bool HasComponent() {
			return HasComponent( typeid(T) );
		}

        /**
         * Creates a copy of this entity.
         * 
         * @return the copy of this entity
         */
        std::shared_ptr<Entity> Clone() {
            auto result = std::make_shared<Entity>();
            for (auto & cmp : compMap) {
                result->AddComponent(cmp.second->Clone());
            }

            return result;
        }

    private:
		/** Used for fast access to components. */
		std::unordered_map<std::type_index, std::shared_ptr<EntityComponent>> compMap;
    };

    /////////////////////////////////////////////////
    /////// EntityFamily
    /////////////////////////////////////////////////

    /**
     * Describes entities which share a certain set of entity components.
     * 
     * @ingroup ecs_group
     */
    class EntityFamily {
    public:

        bool IsMember(const Entity & entity) const
        {
            for (const auto & type : types) {
                if (!entity.HasComponent(type)) {
                    return false;
                }
            }

            return true;
        }

		/**
		 * Creates a new entity family. This static factory method is the only
         * way to construct entity families. The constructor of this class is
         * private.
		 *
		 * Example usage:
		 * ```
		 * EntityFamily myFamily 
         *  = EntityFamily::create<Transform2D, ShapeVisual2D>();
		 * ```
		 *
		 * @tparam Ts list of components the requested entities should share
		 * @return the entity family
		 */
		template<typename ...Ts> static EntityFamily Create() {
			EntityFamily result;
			result.AddType(std::type_index(typeid(Ts))...);
			return result;
		}

        /**
         * Binary less operator.
         * 
         * This operator is required to work as key in a map container.
         * The less operator induces a strict weak ordering of elements.
         *
         * @param o the other entity family (right hand side)
         * @return `true` if this element is less than the given element.
         */
        bool operator <(const EntityFamily& o) const {
            return types < o.types;
        }

    private:
        /** The component types required to be a member of this family. */
        std::set<std::type_index> types;

        /**
         * Constructor.
         * 
         * Private to force use of static factory method.
         */
        EntityFamily() {
        // Intentionally left empty.
        }

		template<typename ... Tx> void AddType(
            const std::type_index & typeIndex, Tx... tx)
        {
			types.insert(typeIndex);
			AddType(tx...);
		}

		/**
		 * Required to stop recursive call of addType, does nothing.
		 */
		void AddType() {}
    };

    /////////////////////////////////////////////////
    /////// EntityView
    /////////////////////////////////////////////////

    using EntityView = std::vector<std::shared_ptr<astu::Entity>>;

    /////////////////////////////////////////////////
    /////// IEntityListener
    /////////////////////////////////////////////////

	/**
	 * Interface for entity listeners which get informed when entities get
	 * added or removed.
     * 
     * @ingroup ecs_group
	 */
    class IEntityListener {
    public:
        
        /**
         * Virtual destructor.
         */
        virtual ~IEntityListener() {}


        /**
         * Called when an entity has been added.
         * 
         * @param entity    the entity which has been added
         */
        virtual void OnEntityAdded(std::shared_ptr<Entity> entity) = 0;

        /**
         * Called when an entity has been removed.
         * 
         * @param entity    the entity which has been removed
         */
        virtual void OnEntityRemoved(std::shared_ptr<Entity> entity) = 0;
    };


    /////////////////////////////////////////////////
    /////// EntityService
    /////////////////////////////////////////////////

    /**
     * The core service of the ECS, manages entities and entity listeners.
     * 
     * @ingroup ecs_group
     */
    class EntityService final : public virtual Service, private Updatable {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority    the update priority of this service
         */
        EntityService(int updatePriority = Priority::Normal);

        /**
         * Adds an entity to this service.
         * 
         * @param entity    the entity to add
         */
        void AddEntity(std::shared_ptr<Entity> entity);

		/**
		 * Removes an entity from this service.
		 *
		 * @param entity	the entity to remove
		 */
		void RemoveEntity(std::shared_ptr<Entity> entity);

		/**
		 * Removes an entity from this service.
		 *
		 * @param entity	the entity to remove
		 */
        void RemoveEntity(Entity & e) {
            RemoveEntity(e.shared_from_this());
        }

        /**
         * Removes all entities.
         */
        void RemoveAll();

        /**
         * Returns a view to a certain family of entities.
         * 
         * Any caller of this method can keep the returned pointer to the entity view.
         * The view gets updated automatically when entities are added or removed.
         * 
         * @return the entity view
         */
        const std::shared_ptr<EntityView> GetEntityView(const EntityFamily & family);

        /**
         * Tests whether an entity listener has already been added.
         * 
         * @param family    the entity family the listener is interested in
         * @param listener  the entity listener to test
         */
        bool HasEntityListener(const EntityFamily & family, IEntityListener & listener) const;

        /**
         * Adds an entity listener to this service.
         * 
         * @param family    the entity family the listener is interested in
         * @param listener  the entity listener to add
         */
        void AddEntityListener(const EntityFamily & family, IEntityListener & listener);

        /**
         * Removes an entity listener to from service.
         * 
         * @param family    the entity family the listener is interested in
         * @param listener  the entity listener to remove
         */
        void RemoveEntityListener(const EntityFamily & family, IEntityListener & listener);

    protected:
        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;

    private:

        using ListenerList = std::vector<IEntityListener*>;

        /** Pending commands. */
        CommandQueue commands;

		/** The entities administered by this service. */
		std::vector<std::shared_ptr<Entity>> entities;        

        /** Maps entity families to corresponding vectors of entities. */
        std::map<EntityFamily, std::shared_ptr<EntityView>> viewMap;

		/** The entity listeners. */
		std::map<EntityFamily, ListenerList> listeners;

        /** Indicates whether an event is currently fired. */
        bool firing;

        void AddEntityInternally(std::shared_ptr<Entity> entity);
        void RemoveEntityInternally(std::shared_ptr<Entity> entity);
        void RemoveFromView(EntityView & view, std::shared_ptr<Entity> entity);
        void RemoveAllInternally();
        
        void FireEntityAdded(ListenerList & listeners, std::shared_ptr<Entity> entity);
        void FireEntityRemoved(ListenerList & listeners, std::shared_ptr<Entity> entity);
    };

} // end of namespace