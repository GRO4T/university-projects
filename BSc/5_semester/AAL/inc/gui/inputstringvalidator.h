#ifndef INPUTSTRINGVALIDATOR_H
#define INPUTSTRINGVALIDATOR_H

#include <QValidator>
#include <QString>

class InputStringValidator : public QValidator {
public:
    InputStringValidator(QObject* parent = nullptr);

    QValidator::State validate(QString& str, int& pos) const override; 
};

#endif // INPUTSTRINGVALIDATOR_H
