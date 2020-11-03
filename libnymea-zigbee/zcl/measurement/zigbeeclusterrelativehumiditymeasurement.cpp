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
    ZigbeeClusterAttribute humidityAttribute = attribute(ZigbeeClusterRelativeHumidityMeasurement::AttributeMeasuredValue);
    return humidityAttribute.dataType().toUInt16() / 100.0;
}

void ZigbeeClusterRelativeHumidityMeasurement::setAttribute(const ZigbeeClusterAttribute &attribute)
{
    qCDebug(dcZigbeeCluster()) << "Update attribute" << m_node << m_endpoint << this << static_cast<Attribute>(attribute.id()) << attribute.dataType();
    if (hasAttribute(attribute.id())) {
        m_attributes[attribute.id()] = attribute;
        emit attributeChanged(attribute);
    } else {
        m_attributes.insert(attribute.id(), attribute);
        emit attributeChanged(attribute);
    }

    // Parse the information for convinience
    if (attribute.id() == AttributeMeasuredValue) {
        bool valueOk = false;
        quint16 value = attribute.dataType().toUInt16(&valueOk);
        if (valueOk) {
            double humidity = value / 100.0;
            qCDebug(dcZigbeeCluster()) << "Humidity changed on" << m_node << m_endpoint << this << humidity << "%";
            emit humidityChanged(humidity);
        }
    }
}
