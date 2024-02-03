//
// Created by damiankolaska on 11/30/21.
//

#ifndef INC_21Z_PAIN_QT2_COMMAND_EXECUTOR_H
#define INC_21Z_PAIN_QT2_COMMAND_EXECUTOR_H

class CommandExecutor {
public:
    static QByteArray execute(const QString& cmd) {
        QProcess process;
        process.start("bash", QStringList() << "-c" << cmd);
        if (!process.waitForStarted())
            throw std::runtime_error("command start timeout");
        if (!process.waitForFinished())
            throw std::runtime_error("command finish timeout");
        return process.readAllStandardOutput();
    }
};

#endif //INC_21Z_PAIN_QT2_COMMAND_EXECUTOR_H
