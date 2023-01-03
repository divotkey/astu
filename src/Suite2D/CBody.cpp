/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/CBody.h"

namespace astu::suite2d {


    CBodyBuilder::CBodyBuilder(std::shared_ptr<CBodyFactory> factory)
        : bodyFactory(factory)
    {
        if (!factory) {
            bodyFactory = ASTU_GET_SERVICE(CBodyFactory);
        }

        Reset();
    }

    std::shared_ptr<CBody> CBodyBuilder::Build()
    {
        auto body = bodyFactory->CreateBody();
        body->SetType(type);
        body->SetLinearVelocity(linearVelocity);
        body->SetAngularVelocity(angularVelocity);
        body->SetAngularDamping(angularDamping);
        body->SetLinearDamping(linearDamping);

        return body;
    }


} // end of namespace