#pragma once

#include "UpdateService.h"
#include "SignalService.h"
#include "Events.h"

namespace astu {

    class Updatable : public IUpdatable, private IServiceListener {
    public:

        Updatable(int updatePriority = 0)
            : updatePriority(updatePriority)
        {
            ASTU_SERVICE(ServiceEventService).AddListener(this);
        }

        virtual ~Updatable() {
            ASTU_SERVICE(ServiceEventService).RemoveListener(this);
        }

    public:

        virtual int GetUpdatePriority() const override {
            return updatePriority;
        }

    private:
        int updatePriority;

        // Inherited via IServiceListener
        virtual void OnSignal(const ServiceEvent & event) override {

            if (dynamic_cast<Updatable*>(&event.service) == this) {

                auto & updateSrv = ASTU_SERVICE(UpdateService);
                switch (event.type) {
                case ServiceEvent::Started:
                    updateSrv.AddUpdatable( event.service.shared_as<IUpdatable>() );
                    break;

                case ServiceEvent::Stopped:
                    updateSrv.RemoveUpdatable( event.service.shared_as<IUpdatable>() );
                    break;
                }
            }
        }
    };

} // end of namespace