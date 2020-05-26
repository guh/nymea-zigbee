/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea-zigbee.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "zigbeedeviceobjectreply.h"

ZigbeeDeviceObjectReply::ZigbeeDeviceObjectReply(const ZigbeeNetworkRequest &request, QObject *parent) :
    QObject(parent),
    m_request(request)
{

}

ZigbeeDeviceObjectReply::Error ZigbeeDeviceObjectReply::error() const
{
    return m_error;
}

ZigbeeNetworkRequest ZigbeeDeviceObjectReply::request() const
{
    return m_request;
}

quint8 ZigbeeDeviceObjectReply::transactionSequenceNumber() const
{
    return m_transactionSequenceNumber;
}

ZigbeeDeviceProfile::ZdoCommand ZigbeeDeviceObjectReply::expectedResponse() const
{
    return m_expectedResponse;
}

QByteArray ZigbeeDeviceObjectReply::responseData() const
{
    return m_responseData;
}

ZigbeeDeviceProfile::Adpu ZigbeeDeviceObjectReply::responseAdpu() const
{
    return m_responseAdpu;
}

Zigbee::ZigbeeApsStatus ZigbeeDeviceObjectReply::zigbeeApsStatus() const
{
    return m_zigbeeApsStatus;
}

bool ZigbeeDeviceObjectReply::isComplete() const
{
    return m_apsConfirmReceived && m_zdpIndicationReceived;
}