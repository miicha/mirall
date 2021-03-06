/*
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#ifndef PROGRESSDISPATCHER_H
#define PROGRESSDISPATCHER_H

#include <QObject>
#include <QHash>
#include <QTime>
#include <QQueue>
#include <QElapsedTimer>

namespace Mirall {


/**
 * @brief The FolderScheduler class schedules folders for sync
 */
namespace Progress
{
    typedef enum {
        Invalid,
        StartSync,
        Download,
        Upload,
        Context,
        Inactive,
        StartDownload,
        StartUpload,
        EndDownload,
        EndUpload,
        EndSync,
        StartDelete,
        EndDelete,
        Error
    } Kind;

    typedef struct {
        Kind    kind;
        QString folder;
        QString current_file;
        qint64  file_size;
        qint64  current_file_bytes;

        qint64  overall_file_count;
        qint64  current_file_no;
        qint64  overall_transmission_size;
        qint64  overall_current_bytes;

        QDateTime timestamp;

    } Info;

    typedef struct {
        QString folder;
        QString current_file;
        QString error_message;
        int     error_code;
        QDateTime  timestamp;
    } SyncProblem;

    QString asActionString( Kind );
    QString asResultString( Kind );
}

/**
 * @file progressdispatcher.h
 * @brief A singleton class to provide sync progress information to other gui classes.
 *
 * How to use the ProgressDispatcher:
 * Just connect to the two signals either to progress for every individual file
 * or the overall sync progress.
 *
 */
class ProgressDispatcher : public QObject
{
    Q_OBJECT

    friend class Folder; // only allow Folder class to access the setting slots.
public:
    static ProgressDispatcher* instance();
    ~ProgressDispatcher();

    QList<Progress::Info> recentChangedItems(int count);
    QList<Progress::SyncProblem> recentProblems(int count);

    Progress::Kind currentFolderContext( const QString& folder );
signals:
    /**
      @brief Signals the progress of data transmission.

      @param[out]  folder The folder which is being processed
      @param[out]  newProgress   A struct with all progress info.

     */

    void progressInfo( const QString& folder, const Progress::Info& progress );
    void progressSyncProblem( const QString& folder, const Progress::SyncProblem& problem );

protected:
    void setProgressInfo(const QString &folder, const Progress::Info& progress);

private:
    ProgressDispatcher(QObject* parent = 0);
    const int _QueueSize;
    QList<Progress::Info> _recentChanges;
    QList<Progress::SyncProblem> _recentProblems;

    QHash<QString, Progress::Kind> _currentAction;

    QElapsedTimer _timer;
    static ProgressDispatcher* _instance;
};

}
#endif // PROGRESSDISPATCHER_H
