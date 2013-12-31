#ifndef CALLSTACKMUX_H
#define CALLSTACKMUX_H

#include <QObject>

#include "quickfindmux.h"

// Interface representing a widget searchable in both direction.
class CallStackInterface {
  public:
    virtual void searchCallStack() = 0;
};

// Interface representing the selector. It will be called and asked
// who the search have to be forwarded to.
class CallStackMuxSelectorInterface {
  public:
    virtual CallStackInterface* getMainView() const = 0;
};

class CallStackMux : public QObject
{
    Q_OBJECT
public:
    explicit CallStackMux(CallStackMuxSelectorInterface* selector_);
    void registerSearchable( QObject* searchable );
    
signals:
    
public slots:
    void searchCallStack();
private:
    CallStackInterface* getMainView() const;
    const CallStackMuxSelectorInterface* selector_;
};

#endif // CALLSTACKMUX_H
