/*
 * Game Boilerplate
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// AST Utilities includes
#include <AstuECS.h>

class CMyComponent : public astu::EntityComponent {
public: 
	// TODO add data members

	/**
	 * Constructor.
	 */
	CMyComponent() {
		// Intentionally left empty.
	}

	// Inherited via EntityComponent
	virtual std::shared_ptr<EntityComponent> Clone() override {
		// Create copy using copy-constructor.
		return std::make_shared<CMyComponent>(*this);
	}    
};