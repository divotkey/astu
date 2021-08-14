/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/VeloxItem.h"
#include "Velox/VeloxScope.h"
#include "Velox/VeloxHook.h"

// C++ Standard Library includes
#include <cmath>

namespace astu {

	class OneRealParamHook : public VeloxHook {
	public:

		/**
		 * Constructor.
		 */
		OneRealParamHook(const std::string & name) : VeloxHook(name) { addParam("a"); }

		// Inherited via VeloxHook
		virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxNode> root, VeloxScope & scope) final override {
			return VeloxItem::createRealValue(calcValue(scope.getItem("a")->getRealValue()));
		}

	protected:

		virtual double calcValue(double a) = 0;
	};

	class TwoRealParamHook : public VeloxHook {
	public:

		/**
		 * Constructor.
		 */
		TwoRealParamHook(const std::string & name) : VeloxHook(name) { addParams("a", "b"); /* addParam("a"); addParam("b");*/ }

		// Inherited via VeloxHook
		virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxNode> root, VeloxScope & scope) final override {
			return VeloxItem::createRealValue(
				calcValue(scope.getItem("a")->getRealValue(), scope.getItem("b")->getRealValue())
			);
		}

	protected:

		virtual double calcValue(double a, double b) = 0;
	};

	class RealVeloxHook : public VeloxHook {
	public:

		RealVeloxHook() : VeloxHook("real") { addParam("a"); }

		// Inherited via VeloxHook
		virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxNode> root, VeloxScope & scope) final override {
			auto value = scope.getItem("a");
			switch (value->getType()) {
			case VeloxItemType::INT:
				return VeloxItem::createRealValue(value->getIntValue());
			case VeloxItemType::REAL:
				return VeloxItem::createRealValue(value->getRealValue());

			default:
				throw VeloxInterpreterException("integer or real value expected");
			}
		}
	};

	class IntVeloxHook : public VeloxHook {
	public:

		IntVeloxHook() : VeloxHook("int") { addParam("a"); }

		// Inherited via VeloxHook
		virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxNode> root, VeloxScope & scope) final override {
			auto value = scope.getItem("a");
			switch (value->getType()) {
			case VeloxItemType::INT:
				return VeloxItem::createIntValue(value->getIntValue());
			case VeloxItemType::REAL:
				return VeloxItem::createIntValue(static_cast<int>(value->getRealValue()));

			default:
				throw VeloxInterpreterException("integer or real value expected");
			}
		}
	};

	class SqrtVeloxHook : public OneRealParamHook {
	public:
		SqrtVeloxHook() : OneRealParamHook("sqrt") {}

	protected:

		// Inherited via VeloxHook
		virtual double calcValue(double a) { return sqrt(a); }
	};

	class SinVeloxHook : public OneRealParamHook {
	public:
		SinVeloxHook() : OneRealParamHook("sin") {}

	protected:

		// Inherited via VeloxHook
		virtual double calcValue(double a) { return sin(a); }
	};

	class CosVeloxHook : public OneRealParamHook {
	public:
		CosVeloxHook() : OneRealParamHook("cos") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a) { return cos(a); }
	};

	class TanVeloxHook : public OneRealParamHook {
	public:
		TanVeloxHook() : OneRealParamHook("tan") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a) { return tan(a); }
	};

	class AtanVeloxHook : public OneRealParamHook {
	public:
		AtanVeloxHook() : OneRealParamHook("atan") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a) { return atan(a); }
	};

	class Atan2VeloxHook : public TwoRealParamHook {
	public:
		Atan2VeloxHook() : TwoRealParamHook("atan2") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a, double b) { return atan2(a, b); }
	};

	class LogVeloxHook : public OneRealParamHook {
	public:
		LogVeloxHook() : OneRealParamHook("log") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a) { return log(a); }
	};

	class CeilVeloxHook : public OneRealParamHook {
	public:
		CeilVeloxHook() : OneRealParamHook("ceil") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a) { return ceil(a); }
	};

	class FloorVeloxHook : public OneRealParamHook {
	public:
		FloorVeloxHook() : OneRealParamHook("floor") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a) { return floor(a); }
	};

	class ToRadiansVeloxHook : public OneRealParamHook {
	public:
		ToRadiansVeloxHook() : OneRealParamHook("toRadians") {}

	protected:
		static const double TO_RADIANS;

		// Inherited via VeloxHook
		virtual double calcValue(double a) { return a * TO_RADIANS; }
	};

	class ToDegreesVeloxHook : public OneRealParamHook {
	public:
		ToDegreesVeloxHook() : OneRealParamHook("toDegrees") {}

	protected:
		static const double TO_DEGREES;

		// Inherited via VeloxHook
		virtual double calcValue(double a) { return a * TO_DEGREES; }
	};

	class PowVeloxHook : public TwoRealParamHook {
	public:
		PowVeloxHook() : TwoRealParamHook("pow") {}

	protected:
		// Inherited via VeloxHook
		virtual double calcValue(double a, double b) { return pow(a, b); }
	};

	//class FindElemVeloxHook : public VeloxHook {
	//public:

	//	FindElemVeloxHook() : VeloxHook("find") { addParam("a"); }

	//	// Inherited via VeloxHook
	//	virtual std::shared_ptr<VeloxItem> invoke(std::shared_ptr<VeloxScript> script, VeloxScope & scope) final override {
	//		auto item = scope.findItem("this");
	//		assert(item->getType() == VeloxItemType::ARRAY);

	//		auto value = scope.getItem("a");
	//		switch (value->getType()) {
	//		case VeloxItemType::INT:
	//			return VeloxItem::createRealValue(value->getIntValue());
	//		case VeloxItemType::REAL:
	//			return VeloxItem::createRealValue(value->getRealValue());

	//		default:
	//			throw VeloxInterpreterException("integer or real value expected");
	//		}
	//	}
	//};


} // end of namespace