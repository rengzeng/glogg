#ifndef CALLSTACK_H
#define CALLSTACK_H
#include <QObject>
#include "selection.h"
#include <QStack>

#define sEntry5  "Entry5:";
#define sExit5 "Exit5:";

class AbstractLogData;
class CallStack  : public QObject
{
    Q_OBJECT
public:
    CallStack( const AbstractLogData* const logData, Selection* selection);
    QStack<qint64> getStack();
    void searchCallStack();
    void disable();
    bool isEnabled();
private:
    void doSearchCallStack (const FilePosition &start_position, const int maxMatch);

    enum LogType { Entry5, Exit5 };
    QString LogType2Str(LogType ltype) const;
    bool isLineMatching(const QString& line, LogType ltype ) const;
    QString funcNameOfLine(QString sLine, LogType ltype) const;
    QString threadOfLine(QString sLine) const;

    // Pointers to external objects
    const AbstractLogData* const logData_;
    Selection* selection_;
    QStack<qint64> stack_;
    bool enabled;
};

#endif // CALLSTACK_H
