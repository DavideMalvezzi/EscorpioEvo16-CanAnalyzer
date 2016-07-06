#include "binaryvalidator.h"

BinaryValidator::BinaryValidator(QObject* parent) : QValidator(parent){

}

QValidator::State BinaryValidator::validate(QString &input, int &pos) const{
    Q_UNUSED(pos);
    QRegExp rx("[0-1]{0,64}");
    if (rx.exactMatch(input)) {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}

