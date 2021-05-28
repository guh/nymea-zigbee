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

#include "zigbeeclusterrelativehumiditymeasurement.h"
#include "zigbeenetworkreply.h"
#include "loggingcategory.h"
#include "zigbeenetwork.h"

ZigbeeClusterRelativeHumidityMeasurement::ZigbeeClusterRelativeHumidityMeasurement(ZigbeeNetwork *network, ZigbeeNode *node, ZigbeeNodeEndpoint *endpoint, Direction direction, QObject *parent) :
    ZigbeeCluster(network, node, endpoint, ZigbeeClusterLibrary::ClusterIdRelativeHumidityMeasurement, direction, parent)
{

}

double ZigbeeClusterRelativeHumidityMeasurement::humidity() const
{
    return m_humidity;
}

void ZigbeeClusterRelativeHumidityMeasurement::setAttribute(const ZigbeeClusterAttribute &attribute)
{
    qCDebug(dcZigbeeCluster()) << "Update attribute" << m_node << m_endpoint << this << static_cast<Attribute>(attribute.id()) << attribute.dataType();
    updateOrAddAttribute(attribute);

    // Parse the information for convenience
    if (attribute.id() == AttributeMeasuredValue) {
        bool valueOk = false;
        quint16 value = attribute.dataType().toUInt16(&valueOk);
        if (valueOk) {
            if (value == 0xffff) {
                qCDebug(dcZigbeeCluster()) << m_node << m_endpoint << this << "received invalid measurement value. Not updating the attribute.";
                return;
            }

            m_humidity = value / 100.0;
            qCDebug(dcZigbeeCluster()) << "Humidity changed on" << m_node << m_endpoint << this << m_humidity << "%";
            emit humidityChanged(m_humidity);
        }
    }
}
