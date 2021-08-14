/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/VeloxTypes.h"
#include "Velox/VeloxItem.h"

// C++ Standard Library includes
#include <vector>
#include <memory>
#include <functional>

namespace astu {

	// Forward declaration.
	class VeloxItem;
	class VeloxScope;
	class VeloxScript;
	class Plum;

	/**
	 * Hooks a representing built-in functions which can be used by scripts.
	 */
	class VeloxHook {
	public:

		VeloxHook(const std::string & name);

		/**
		 * Returns the name of this hook.
		 *
		 * @return the name
		 */
		const std::string & getName() const;

		/**
		 * Invokes this hook function.
		 *
		 * @pram root		the root node of the velox AST calling this hook
		 * @pram scope		the scope of this hook, containing parameters etc.
		 */
		virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxNode> root, VeloxScope & scope) = 0;

		/**
		 * Returns an iterator pointing to the first formal parameter.
		 *
		 * @return iterator pointing at the first formal parameter
		 */
		std::vector<std::string>::iterator begin() {
			return m_formalParams.begin();
		}

		/**
		 * Returns an iterator referring to the past-the-end element in the formal parameter list.
		 *
		 * @return iterator to the element past the end of the parameter sequence
		 */
		std::vector<std::string>::iterator end() {
			return m_formalParams.end();
		}

		/**
		 * Returns an iterator pointing to the first formal parameter.
		 *
		 * @return iterator pointing at the first formal parameter
		 */
		std::vector<std::string>::const_iterator begin() const {
			return m_formalParams.cbegin();
		}

		/**
		 * Returns an iterator referring to the past-the-end element in the formal parameter list.
		 *
		 * @return iterator to the element past the end of the parameter sequence
		 */
		std::vector<std::string>::const_iterator end() const {
			return m_formalParams.cend();
		}

		template<typename T, typename... Args>
		void addParams(T param, Args... args) {
			addParam(param);
			addParams(args...);
		}

		template<typename T>
		void addParams(T param) {
			m_formalParams.push_back(param);
		}

		/**
		 * Adds a formal parameter to the parameter list of this hook.
		 *
		 * @param s	the parameter to add
		 */
		void addParam(const std::string & s);

	protected:

		/**
		 * Returns a double parameter with the specified name.
		 *
		 * @return the real parameter
		 * @throws VeloxInterpreterException in case the parameter is missing or has the wrong type
		 */
		double getDouble(VeloxScope & scope, const std::string & name);

		/**
		 * Returns a real parameter with the specified name.
		 *
		 * @return the real parameter
		 * @throws VeloxInterpreterException in case the parameter is missing or has the wrong type
		 */
		Real getReal(VeloxScope & scope, const std::string & name);

	private:
		/** The name of this hook. */
		std::string m_name;

		/** The formal parameter list of this hook function. */
		std::vector<std::string> m_formalParams;
	};


	template <typename T>
	class ContextVeloxHook : public VeloxHook {
	public:

		ContextVeloxHook(T & context, const std::string & name) : VeloxHook(name), m_context(context)  {}

	protected:

		T & getContext() {
			return m_context;
		}

	private:
		/** The context of this hook. */
		T & m_context;
	};

	/**
	 * This hook class can be used in combination with lambda expression 
	 */
	class FunctionVeloxHook final : public VeloxHook {
	public:

		using InvokeFunc = std::function<std::shared_ptr<VeloxItem>(std::shared_ptr<VeloxNode> root, VeloxScope & scope)>;

		FunctionVeloxHook(const std::string & name, InvokeFunc funk)
			: VeloxHook(name), m_func(funk)
		{
			// Intentionally left empty.
		}

		virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxNode> root, VeloxScope & scope) override {
			 return m_func(root, scope);
		}

	private:
		/* The function to be called when this hook is invoked. */
		InvokeFunc m_func;
	};

} // end of namespace