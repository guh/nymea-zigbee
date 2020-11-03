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

#include "zigbeeclusteroccupancysensing.h"
#include "zigbeenetworkreply.h"
#include "loggingcategory.h"
#include "zigbeenetwork.h"

ZigbeeClusterOccupancySensing::ZigbeeClusterOccupancySensing(ZigbeeNetwork *network, ZigbeeNode *node, ZigbeeNodeEndpoint *endpoint, Direction direction, QObject *parent) :
    ZigbeeCluster(network, node, endpoint, ZigbeeClusterLibrary::ClusterIdOccupancySensing, direction, parent)
{

}

bool ZigbeeClusterOccupancySensing::occupancy() const
{
    ZigbeeClusterAttribute occupancyAttribute = attribute(ZigbeeClusterOccupancySensing::AttributeOccupancy);
    return occupancyAttribute.dataType().toBool();
}

void ZigbeeClusterOccupancySensing::setAttribute(const ZigbeeClusterAttribute &attribute)
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
    if (attribute.id() == AttributeOccupancy) {
        bool valueOk = false;
        bool value = attribute.dataType().toBool(&valueOk);
        if (valueOk) {
            qCDebug(dcZigbeeCluster()) << "Occupancy changed on" << m_node << m_endpoint << this << value;
            emit occupancyChanged(value);
        }
    }
}
