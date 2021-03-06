/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef QV8PROFILERDATAMODEL_H
#define QV8PROFILERDATAMODEL_H

#include "qmlprofilerbasemodel.h"
#include <utils/fileinprojectfinder.h>

#include <QFutureInterface>
#include <QObject>
#include <QHash>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace QmlProfiler {

class QV8ProfilerDataModel : public QmlProfilerBaseModel
{
    Q_OBJECT
public:
    struct QV8EventSub;

    struct QV8EventData
    {
        QV8EventData();
        ~QV8EventData();

        QString displayName;
        QString eventHashStr;
        QString filename;
        QString functionName;
        int line;
        double totalTime; // given in milliseconds
        double totalPercent;
        double selfTime;
        double SelfTimeInPercent;
        QHash <QString, QV8EventSub *> parentHash;
        QHash <QString, QV8EventSub *> childrenHash;
        int typeId;

        QV8EventData &operator=(const QV8EventData &ref);
    };

    struct QV8EventSub {
        QV8EventSub(QV8EventData *from) : reference(from), totalTime(0) {}
        QV8EventSub(QV8EventSub *from) : reference(from->reference), totalTime(from->totalTime) {}

        QV8EventData *reference;
        qint64 totalTime;
    };

    QV8ProfilerDataModel(Utils::FileInProjectFinder *fileFinder, QmlProfilerModelManager *parent = 0);

    void clear();
    bool isEmpty() const;
    QList<QV8EventData *> getV8Events() const;
    int numberOfV8Events() const;
    QV8EventData *v8EventDescription(int typeId) const;

    qint64 v8MeasuredTime() const;

    void save(QXmlStreamWriter &stream, QFutureInterface<void> *future = 0);
    void load(QXmlStreamReader &stream, QFutureInterface<void> *future = 0);

    void complete();

public slots:
    void addV8Event(int depth,
                    const QString &function,
                    const QString &filename,
                    int lineNumber,
                    double totalTime,
                    double selfTime);
    void detailsChanged(int requestId, const QString &newString);
    void detailsDone();

private:
    class QV8ProfilerDataModelPrivate;
    void clearV8RootEvent();

    Q_DECLARE_PRIVATE(QV8ProfilerDataModel)
};

} // namespace QmlProfiler

#endif // QV8PROFILERDATAMODEL_H
