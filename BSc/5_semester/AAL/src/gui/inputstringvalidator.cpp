#include "inc/gui/inputstringvalidator.h"
#include <cctype>
#include <cstring>
#include <qnamespace.h>

InputStringValidator::InputStringValidator(QObject* parent)
    : QValidator(parent) {

}
    
QValidator::State InputStringValidator::validate(QString& str, int& pos) const {
    if (str.length() == 0) 
        return QValidator::State::Invalid;

    static const QString cmyk = "CMYK";
    for (const auto & chr : str) {
        if (cmyk.indexOf(chr, 0, Qt::CaseInsensitive) == -1)
            return QValidator::State::Invalid;
    }

    return QValidator::State::Acceptable;
}
