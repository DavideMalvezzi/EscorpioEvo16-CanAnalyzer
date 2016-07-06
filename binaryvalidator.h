#ifndef BINARYVALIDATOR_H
#define BINARYVALIDATOR_H

#include <QValidator>

class BinaryValidator : public QValidator {

public:
    BinaryValidator(QObject*);
    virtual QValidator::State validate(QString &, int &) const;
};


#endif // BINARYVALIDATOR_H
