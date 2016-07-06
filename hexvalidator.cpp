#include "hexvalidator.h"

HexValidator::HexValidator(QObject* parent) : QValidator(parent){

}

QValidator::State HexValidator::validate(QString &input, int &pos) const{
    Q_UNUSED(pos);
    QRegExp rx("[0-9a-fA-F]{0,16}");
    if (rx.exactMatch(input)) {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}

