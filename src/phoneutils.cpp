/*
 * Copyright (C) 2012-2015 Canonical, Ltd.
 *
 * Authors:
 *  Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 *  Renato Araujo Oliveira Filho <renato.filho@canonical.com>
 *  Tiago Salem Herrmann <tiago.herrmann@canonical.com>
 *
 * This file is part of history-service.
 *
 * history-service is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * history-service is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "phoneutils_p.h"

#include <phonenumbers/phonenumbermatch.h>
#include <phonenumbers/phonenumbermatcher.h>
#include <phonenumbers/phonenumberutil.h>

#include <QLocale>
#include <QDebug>

PhoneUtils::PhoneUtils(QObject *parent) :
    QObject(parent)
{
}

QString PhoneUtils::region()
{
    QString countryCode = QLocale::system().name().split("_").last();
    if (countryCode.size() < 2) {
        // fallback to US if no valid country code was provided, otherwise libphonenumber
        // will fail to parse any numbers
        return QString("US");
    }
    return countryCode;
}

QString PhoneUtils::normalizePhoneNumber(const QString &phoneNumber)
{
    static i18n::phonenumbers::PhoneNumberUtil *phonenumberUtil = i18n::phonenumbers::PhoneNumberUtil::GetInstance();
    if (!isPhoneNumber(phoneNumber)) {
        return phoneNumber;
    }
    std::string number = phoneNumber.toStdString();
    phonenumberUtil->NormalizeDiallableCharsOnly(&number);
    return QString::fromStdString(number);
}

bool PhoneUtils::comparePhoneNumbers(const QString &phoneNumberA, const QString &phoneNumberB)
{
    // just do a simple string comparison first
    if (phoneNumberA == phoneNumberB) {
        return true;
    }

    QString normalizedPhoneNumberA = normalizePhoneNumber(phoneNumberA);
    QString normalizedPhoneNumberB = normalizePhoneNumber(phoneNumberB);

    return compareNormalizedPhoneNumbers(normalizedPhoneNumberA, normalizedPhoneNumberB);
}

bool PhoneUtils::compareNormalizedPhoneNumbers(const QString &numberA, const QString &numberB)
{
    if (numberA == numberB) {
        return true;
    }

    static i18n::phonenumbers::PhoneNumberUtil *phonenumberUtil = i18n::phonenumbers::PhoneNumberUtil::GetInstance();

    if (numberA.size() < 7 || numberB.size() < 7) {
        return false;
    }

    i18n::phonenumbers::PhoneNumberUtil::MatchType match = phonenumberUtil->
            IsNumberMatchWithTwoStrings(numberA.toStdString(),
                                        numberB.toStdString());
    return (match > i18n::phonenumbers::PhoneNumberUtil::NO_MATCH);
}

bool PhoneUtils::isPhoneNumber(const QString &phoneNumber)
{
    static i18n::phonenumbers::PhoneNumberUtil *phonenumberUtil = i18n::phonenumbers::PhoneNumberUtil::GetInstance();
    i18n::phonenumbers::PhoneNumber number;
    i18n::phonenumbers::PhoneNumberUtil::ErrorType error;
    error = phonenumberUtil->Parse(phoneNumber.toStdString(), region().toStdString(), &number);

    switch(error) {
    case i18n::phonenumbers::PhoneNumberUtil::INVALID_COUNTRY_CODE_ERROR:
        qWarning() << "Invalid country code for:" << phoneNumber;
        return false;
    case i18n::phonenumbers::PhoneNumberUtil::NOT_A_NUMBER:
        qWarning() << "The phone number is not a valid number:" << phoneNumber;
        return false;
    case i18n::phonenumbers::PhoneNumberUtil::TOO_SHORT_AFTER_IDD:
    case i18n::phonenumbers::PhoneNumberUtil::TOO_SHORT_NSN:
    case i18n::phonenumbers::PhoneNumberUtil::TOO_LONG_NSN:
        qWarning() << "Invalid phone number" << phoneNumber;
        return false;
    default:
        break;
    }
    return true;
}

