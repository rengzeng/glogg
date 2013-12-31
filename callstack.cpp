#include "callstack.h"
#include "log.h"
#include <QPair>


CallStack::CallStack( const AbstractLogData* const logData, Selection* selection) :
    QObject(),  logData_( logData ), selection_( selection )
{
    enabled = true;
}


void CallStack::searchCallStack()
{
    LOG(logINFO) << "Entering CallStack::searchCallStack";

    // Position where we start the search from
    FilePosition start_position = selection_->getPreviousPosition();

    doSearchCallStack( start_position, 10 );

}

QString CallStack::LogType2Str(
        LogType ltype) const
{
    QString sMatch;
    switch (ltype) {
    case Entry5:
        sMatch = sEntry5;
        break;
    case Exit5:
        sMatch = sExit5;
        break;
    default:
        sMatch = "";
        LOG(logERROR) << "invalid log type";
        break;
    }
    return sMatch;
}

bool CallStack::isLineMatching(
        const QString& line , LogType ltype) const
{
    if ( line.contains(LogType2Str(ltype), Qt::CaseSensitive) ) {
        return true;
    }
    else {
        return false;
    }
}


QString CallStack::funcNameOfLine(QString sLine, LogType ltype) const
{
    QStringList sl = sLine.split(LogType2Str(ltype));
    if (sl.size() == 2)
        return sl.at(1); // Assume the text following Entry5/Exit5 is the function name
    else
        return "Error: No func name found";
}

QString CallStack::threadOfLine(QString sLine) const
{
    QStringList sl = sLine.split("]"); //Assume the format of each line is: [   d] ...
    return sl.at(0);
}

void CallStack::doSearchCallStack (const FilePosition &start_position, const int maxMatch)
{
    int found = 0;


    qint64 line = start_position.line();
    LOG( logDEBUG ) << "Start searching at line " << line;

    stack_.clear();
    QStack<QPair<qint64,QString> > stackExit;

    QString thd = threadOfLine(logData_->getExpandedLineString( line ));
    line--;
    while (thd.at(0) != '[')
    {
        // Find the line starting with thread number such as [   d]
        thd = threadOfLine(logData_->getExpandedLineString( line ));
        line--;
    }

    while ( line >= 0 ) {
        QString sLine = logData_->getExpandedLineString( line );
        if (threadOfLine(sLine) != thd)
        {
            line--;
            continue;
        }
        if ( isLineMatching( sLine, Entry5 ) ) {
            if ((!stackExit.isEmpty()) && (funcNameOfLine(sLine, Entry5) == stackExit.top().second))
            {
                qint64 exitLine = stackExit.pop().first;
                // Just in case "exit" can be missing in the log
                while (stack_.top() < exitLine) {
                    found--;
                    stack_.pop();
                }
            }
            else
            {
                stack_.push(line);
                found++;
                if (found == maxMatch) break;
            }
        }
        else if (isLineMatching( sLine, Exit5 )) {
            stackExit.push(qMakePair(line, funcNameOfLine(sLine, Exit5)));
        }
        line--;
    }

    return;
}

QStack<qint64> CallStack::getStack()
{
    return stack_;
}

void CallStack::disable()
{
    enabled = false;
}

bool CallStack::isEnabled()
{
    return enabled;
}
