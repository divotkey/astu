/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Velox/VeloxScript.h"
#include "Velox/VeloxInterpreter.h"
#include "Velox/VeloxAst.h"
#include "Velox/VeloxItem.h"
#include "IVeloxImporter.h"
#include "Velox/VeloxScope.h"

// C++ Standard Library includes
#include <iostream>

#define LOG_TAG "VLX"
#define LOG(tag, msg) cout << tag << ": " << msg << endl;

using namespace std;

namespace astu {

	VeloxScript::VeloxScript(std::shared_ptr<VeloxNode> node, const std::string & sourcePath)
		: m_node(node), m_sourcePath(sourcePath)
	{
		PREPARE_POOLING();
		m_interpreter = VeloxInterpreter::Obtain(node, sourcePath);
		m_interpreter->setPrintFunc([](const std::string& s){ cout << s << endl;});
	}

	VeloxScript::~VeloxScript()
	{
		if (m_interpreter) {
			VeloxInterpreter::Free(move(m_interpreter));
		}
	}

	std::shared_ptr<VeloxItem> VeloxScript::evaluate(VeloxItem & function)
	{
		ensureInitialized();

		try {
			return m_interpreter->evaluate(function);
		}
		catch (VeloxInterpreterException & e) {
			LOG(LOG_TAG, "script runtime error: "+ e.getMessage());
			return VeloxItem::createUndefined();
		}
	}

	std::shared_ptr<VeloxItem> VeloxScript::evaluate(VeloxItem & function, const FunctionParameters & parameters)
	{
		ensureInitialized();

		try {
			return m_interpreter->evaluate(function, parameters);
		}
		catch (VeloxInterpreterException & e) {
			LOG(LOG_TAG, "script runtime error: " + e.getMessage());
			return VeloxItem::createUndefined();
		}
	}

	void VeloxScript::execute()
	{
		ensureInitialized();
		try {
			m_interpreter->execute();
		}
		catch (VeloxInterpreterException & e) {
			LOG(LOG_TAG, "script runtime error: " + e.getMessage());
		}
	}

	const std::string & VeloxScript::getSourcePath() const
	{
		return m_sourcePath;
	}

	void VeloxScript::ensureInitialized()
	{
		if (!m_interpreter) {
			throw std::logic_error("Velox script not initialized");
		}
	}

	/////////////////////////////////////////////////
	/////// class Parameters
	/////////////////////////////////////////////////

	int FunctionParameters::m_nameCounter = 0;

	FunctionParameters::FunctionParameters()
		: m_parameters(VeloxNode::create(VeloxNodeType::FUNCTION_PARAMS))
	{
		// Intentionally left empty.
	}

	void FunctionParameters::addParam(std::shared_ptr<VeloxItem> item)
	{
		auto param = VeloxNode::create(VeloxNodeType::SIMPLE_NAME);
		param->setStringData(genName());
		m_parameters->addChild(param);
		m_items.push_back(item);
	}

	void FunctionParameters::addParam(int value)
	{
		auto param = VeloxNode::create(VeloxNodeType::INTEGER_LITERAL);
		param->setIntegerData(value);
		m_parameters->addChild(param);
	}

	void FunctionParameters::addParam(Real value)
	{
		auto param = VeloxNode::create(VeloxNodeType::REAL_LITERAL);
		param->setRealData(static_cast<double>(value));
		m_parameters->addChild(param);
	}

	void FunctionParameters::addParam(const std::string & s)
	{
		auto param = VeloxNode::create(VeloxNodeType::STRING_LITERAL);
		param->setStringData(s);
		m_parameters->addChild(param);
	}

	const VeloxNode & FunctionParameters::getParamNode() const
	{
		return *m_parameters;
	}

	const std::vector<std::shared_ptr<VeloxItem>>& FunctionParameters::getItems() const
	{
		return  m_items;
	}

	void FunctionParameters::clear()
	{
		m_items.clear();
		m_parameters->removeChildren();
	}

	std::string FunctionParameters::genName()
	{
		return "_param" + std::to_string(++m_nameCounter);
	}

} // end of namespace