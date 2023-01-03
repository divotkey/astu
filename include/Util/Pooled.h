/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <cassert>
#include <algorithm>
#include <vector>
#include <memory>
#include <new>
#include <cstdlib>
#include <iostream>	// for stop-gap logging facility.

#define LOG_WARNING(a) (std::cout << "WARNING: " << (a) << std::endl)

#ifdef NDEBUG
	#define PREPARE_POOLING()
#else
	#define PREPARE_POOLING() m_this = this
#endif

namespace astu {

	/**
	 * A template for pooled objects.
	 * 
	 * @tparam T	the type of the object to be pooled
	 * @ingroup misc_group
	 */
	template<typename T>
	class Pooled {
	public:

		/**
		 * Returns a new instance.
		 * 
		 * @return the newly created or reused instance
		 */
		static std::unique_ptr<T> Create() {
			return std::unique_ptr<T>(new T);
		}

		/**
		 * Returns the number of free items in the item pool.
		 *
		 * @return the number of free items
		 */
		static size_t NumberOfPooledItems() {
			return ms_rawPool.size();
		}

		/**
		 * Returns the number of used items taken from the item pool.
		 * This method returns always zero in debug mode.
		 *
		 * @return the number of used items
		 */
		static size_t NumberOfUsedItems() {
			return ms_rawUsed.size();
		}

		/**
		 * Destructor.
		 */
		virtual ~Pooled() {
			#ifndef NDEBUG
			if (!isUsed(reinterpret_cast<T*>(m_this != 0 ? m_this : this))) {
				LOG_WARNING("pooled instance of type '" 
					+ std::string(typeid(T).name()) 
					+ "' destroyed");
			}
			#endif
		}

		/**
		 * Operator new.
		 *
		 * @param count the number of bytes to allocate
		 */
		void * operator new(size_t count) {
			assert(count == sizeof(T));
			T * result;

			if (ms_rawPool.empty()) {
				result = reinterpret_cast<T*>(std::malloc(count));
				if (!result) {
					throw std::bad_alloc{};
				}
			}
			else {
				result = ms_rawPool.back();
				ms_rawPool.pop_back();
			}
			#ifndef NDEBUG
				addUsed(result);
			#endif
			return result;
		}

		void operator delete(void * p)
		{
			if (p == nullptr) {
				return;
			}
			T * instance = reinterpret_cast<T*>(p);
			ms_rawPool.push_back(instance);
			#ifndef NDEBUG
				removeUsed(instance);
			#endif
		}

	protected:

#ifdef NDEBUG
		/**
		 * Constructor.
		 */
		Pooled() {
			// Intentionally left empty.
		}
#else
		/**
		 * Constructor.
		 */
		Pooled() : m_this(0) {
			// Intentionally left empty.
		}
#endif

	protected:
		#ifndef NDEBUG
		/** Stores the real this pointer used for debugging and pooling verification. */
		void *m_this;
		#endif

	private:
		/** The pool of pooled instances items. */
		static inline std::vector<T *> ms_rawPool;

		/** Used for debugging purpose to verify that items get pooled. */
		static inline std::vector<T *> ms_rawUsed;


		/**
		 * Adds the specified instance to the used list.
		 * This method is solely for debugging purpose
		 *
		 * @param instance	the instance to add
		 */
		static void addUsed(T * instance) {
			assert(!isUsed(instance));
			ms_rawUsed.push_back(instance);
		}

		/**
		 * Removes the specified instance from the used list.
		 * This method is solely for debugging purpose
		 *
		 * @param instance	the instance to remove
		 */
		static void removeUsed(T * instance) {
			ms_rawUsed.erase(
				std::remove(ms_rawUsed.begin(), ms_rawUsed.end(), instance), 
				ms_rawUsed.end()
			);
		}

		/**
		 * Tests if the specified instance is currently in use.
		 * This method is solely for debugging purpose
		 *
		 * @param instance	the instance to test
		 * @return `true` if the instance is currently in use
		 */
		static bool isUsed(T * instance) {
			return find(ms_rawUsed.begin(), ms_rawUsed.end(), instance) 
				!= ms_rawUsed.end();
		}

	};


	// Static template members must be defined here to. Since it is
	// part of a template, as with all templates the compiler will make sure it
	// is only defined once. C++ 17 does alow an inline notation as well.
	// Uncomment this code and remove `inline` keyword in the template static
	// member defintion when using older C++ versions.
	// template <typename T>
	// std::vector<T *> Pooled<T>::ms_rawPool;

	// template <typename T>
	// std::vector<T *> Pooled<T>::ms_rawUsed;

} // end of namespace