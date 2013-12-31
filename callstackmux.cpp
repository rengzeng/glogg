#include "callstackmux.h"
#include "log.h"

CallStackMux::CallStackMux(CallStackMuxSelectorInterface* selector) :
    QObject()
{
    selector_ = selector;
}

void CallStackMux::registerSearchable( QObject* searchable )
{
    LOG(logINFO) << "Entering CallStack::registerSearchable";
    // Search can be initiated by the view itself
    connect( searchable, SIGNAL( searchCallStackFromView() ),
             this, SLOT( searchCallStack() ) );
}

void CallStackMux::searchCallStack()
{
    LOG(logINFO) << "Entering CallStackMux::searchCallStack";

    CallStackInterface* searchable = getMainView();

    searchable->searchCallStack();

}

//
// Private member functions
//

// Use the registered 'selector' to determine where to send the search requests.
CallStackInterface* CallStackMux::getMainView() const
{
    CallStackInterface* searchable = NULL;

    if ( selector_ )
        searchable = selector_->getMainView();
    else
        LOG(logERROR) << "CallStackMux::getMainView() no registered selector";

    return searchable;
}
