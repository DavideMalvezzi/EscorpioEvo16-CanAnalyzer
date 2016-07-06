#ifndef HEXVALIDATOR_H
#define HEXVALIDATOR_H

#include <QValidator>

class HexValidator : public QValidator {

public:
    HexValidator(QObject*);
    virtual QValidator::State validate(QString &, int &) const;
};

#endif // HEXVALIDATOR_H
