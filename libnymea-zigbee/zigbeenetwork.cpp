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

#include "zigbeeutils.h"
#include "zigbeenetwork.h"
#include "loggingcategory.h"
#include "zdo/zigbeedeviceprofile.h"
#include "zigbeebridgecontroller.h"
#include "zigbeenetworkdatabase.h"

#include <QDir>
#include <QFileInfo>
#include <QDataStream>

ZigbeeNetwork::ZigbeeNetwork(const QUuid &networkUuid, QObject *parent) :
    QObject(parent),
    m_networkUuid(networkUuid)
{
    m_permitJoinTimer = new QTimer(this);
    m_permitJoinTimer->setInterval(1000);
    m_permitJoinTimer->setSingleShot(false);
    connect(m_permitJoinTimer, &QTimer::timeout, this, [this](){
        m_permitJoiningRemaining--;
        if (m_permitJoiningRemaining <= 0) {
            m_permitJoinTimer->stop();
            setPermitJoining(0);
        } else {
            setPermitJoiningRemaining(m_permitJoiningRemaining);
        }
    });

    m_reachableRefreshTimer = new QTimer(this);
    m_reachableRefreshTimer->setInterval(120000);
    m_reachableRefreshTimer->setSingleShot(false);
    connect(m_reachableRefreshTimer, &QTimer::timeout, this, &ZigbeeNetwork::evaluateNodeReachableStates);

    connect(this, &ZigbeeNetwork::stateChanged, this, [this](ZigbeeNetwork::State state){
        if (state == ZigbeeNetwork::StateRunning) {
            evaluateNodeReachableStates();
            m_reachableRefreshTimer->start();
        } else {
            foreach (ZigbeeNode *node, m_nodes) {
                node->setReachable(false);
            }
            m_reachableRefreshTimer->stop();
        }
    });
}

QUuid ZigbeeNetwork::networkUuid() const
{
    return m_networkUuid;
}

ZigbeeNetwork::State ZigbeeNetwork::state() const
{
    return m_state;
}

ZigbeeNetwork::Error ZigbeeNetwork::error() const
{
    return m_error;
}

QDir ZigbeeNetwork::settingsDirectory() const
{
    return m_settingsDirectory;
}

void ZigbeeNetwork::setSettingsDirectory(const QDir &settingsDirectory)
{
    qCDebug(dcZigbeeNetwork()) << "Using settings directory" << settingsDirectory.absolutePath();
    m_settingsDirectory = settingsDirectory;
    emit settingsDirectoryChanged(m_settingsDirectory);

    bridgeController()->setSettingsDirectory(m_settingsDirectory);
}

QString ZigbeeNetwork::serialPortName() const
{
    return m_serialPortName;
}

void ZigbeeNetwork::setSerialPortName(const QString &serialPortName)
{
    if (m_serialPortName == serialPortName)
        return;

    m_serialPortName = serialPortName;
    emit serialPortNameChanged(m_serialPortName);
}

qint32 ZigbeeNetwork::serialBaudrate() const
{
    return m_serialBaudrate;
}

void ZigbeeNetwork::setSerialBaudrate(qint32 baudrate)
{
    if (m_serialBaudrate == baudrate)
        return;

    m_serialBaudrate = baudrate;
    emit serialBaudrateChanged(m_serialBaudrate);
}

QString ZigbeeNetwork::serialNumber() const
{
    return m_serialNumber;
}

void ZigbeeNetwork::setSerialNumber(const QString &serialNumber)
{
    m_serialNumber = serialNumber;
}

ZigbeeAddress ZigbeeNetwork::macAddress() const
{
    return m_macAddress;
}

void ZigbeeNetwork::setMacAddress(const ZigbeeAddress &zigbeeAddress)
{
    if (m_macAddress == zigbeeAddress)
        return;

    m_macAddress = zigbeeAddress;
    emit macAddressChanged(m_macAddress);
}

QString ZigbeeNetwork::firmwareVersion() const
{
    if (bridgeController()) {
        return bridgeController()->firmwareVersion();
    } else {
        return QString();
    }
}

quint16 ZigbeeNetwork::panId()
{
    return m_panId;
}

void ZigbeeNetwork::setPanId(quint16 panId)
{
    if (m_panId == panId)
        return;

    m_panId = panId;
    emit panIdChanged(m_panId);
}

quint64 ZigbeeNetwork::extendedPanId() const
{
    return m_extendedPanId;
}

void ZigbeeNetwork::setExtendedPanId(quint64 extendedPanId)
{
    if (m_extendedPanId == extendedPanId)
        return;

    m_extendedPanId = extendedPanId;
    emit extendedPanIdChanged(m_extendedPanId);
}

quint32 ZigbeeNetwork::channel() const
{
    return m_channel;
}

void ZigbeeNetwork::setChannel(quint32 channel)
{
    if (m_channel == channel)
        return;

    m_channel = channel;
    emit channelChanged(m_channel);
}

ZigbeeChannelMask ZigbeeNetwork::channelMask() const
{
    return m_channelMask;
}

void ZigbeeNetwork::setChannelMask(const ZigbeeChannelMask &channelMask)
{
    if (m_channelMask == channelMask)
        return;

    m_channelMask = channelMask;
    emit channelMaskChanged(m_channelMask);
}

ZigbeeSecurityConfiguration ZigbeeNetwork::securityConfiguration() const
{
    return m_securityConfiguration;
}

void ZigbeeNetwork::setSecurityConfiguration(const ZigbeeSecurityConfiguration &securityConfiguration)
{
    if (m_securityConfiguration == securityConfiguration)
        return;

    m_securityConfiguration = securityConfiguration;
    emit securityConfigurationChanged(m_securityConfiguration);
}

bool ZigbeeNetwork::permitJoiningEnabled() const
{
    return m_permitJoiningEnabled;
}

quint8 ZigbeeNetwork::permitJoiningDuration() const
{
    return m_permitJoiningDuration;
}

quint8 ZigbeeNetwork::permitJoiningRemaining() const
{
    return m_permitJoiningRemaining;
}

void ZigbeeNetwork::setPermitJoiningEnabled(bool permitJoiningEnabled)
{
    if (m_permitJoiningEnabled == permitJoiningEnabled)
        return;

    m_permitJoiningEnabled = permitJoiningEnabled;
    emit permitJoiningEnabledChanged(m_permitJoiningEnabled);

    if (!m_permitJoiningEnabled) {
        m_permitJoinTimer->stop();
        setPermitJoiningRemaining(0);
    }
}

void ZigbeeNetwork::setPermitJoiningDuration(quint8 duration)
{
    if (m_permitJoiningDuration == duration)
        return;

    m_permitJoiningDuration = duration;
    emit permitJoinDurationChanged(m_permitJoiningDuration);
}

void ZigbeeNetwork::setPermitJoiningRemaining(quint8 remaining)
{
    if (m_permitJoiningRemaining == remaining)
        return;

    m_permitJoiningRemaining = remaining;
    emit permitJoinRemainingChanged(m_permitJoiningRemaining);
}

quint8 ZigbeeNetwork::generateSequenceNumber()
{
    return m_sequenceNumber++;
}

QList<ZigbeeNode *> ZigbeeNetwork::nodes() const
{
    return m_nodes;
}

ZigbeeNode *ZigbeeNetwork::coordinatorNode() const
{
    return m_coordinatorNode;
}

ZigbeeNode *ZigbeeNetwork::getZigbeeNode(quint16 shortAddress) const
{
    foreach (ZigbeeNode *node, m_uninitializedNodes) {
        if (node->shortAddress() == shortAddress) {
            return node;
        }
    }

    foreach (ZigbeeNode *node, m_nodes) {
        if (node->shortAddress() == shortAddress) {
            return node;
        }
    }

    foreach (ZigbeeNode *node, m_temporaryNodes) {
        if (node->shortAddress() == shortAddress) {
            return node;
        }
    }

    return nullptr;
}

ZigbeeNode *ZigbeeNetwork::getZigbeeNode(const ZigbeeAddress &address) const
{
    foreach (ZigbeeNode *node, m_uninitializedNodes) {
        if (node->extendedAddress() == address) {
            return node;
        }
    }

    foreach (ZigbeeNode *node, m_nodes) {
        if (node->extendedAddress() == address) {
            return node;
        }
    }

    return nullptr;
}

bool ZigbeeNetwork::hasNode(quint16 shortAddress) const
{
    return getZigbeeNode(shortAddress) != nullptr;
}

bool ZigbeeNetwork::hasNode(const ZigbeeAddress &address) const
{
    return getZigbeeNode(address) != nullptr;
}

void ZigbeeNetwork::removeZigbeeNode(const ZigbeeAddress &address)
{
    ZigbeeNode *node = getZigbeeNode(address);
    if (!node) {
        qCWarning(dcZigbeeNetwork()) << "Failed remove zigbee node since there is no node with" << address;
        return;
    }

    qCDebug(dcZigbeeNetwork()) << "Removing" << node << "from the newtork";
    removeNodeInternally(node);

    ZigbeeDeviceObjectReply *zdoReply = node->deviceObject()->requestMgmtLeaveNetwork();
    connect(zdoReply, &ZigbeeDeviceObjectReply::finished, this, [zdoReply, node](){
        if (zdoReply->error() != ZigbeeDeviceObjectReply::ErrorNoError) {
            qCWarning(dcZigbeeNode()) << "Failed to send management leave request to" << node << zdoReply->error();
            qCWarning(dcZigbeeNode()) << "This node is gonna be removed internally. TODO: try to remove using ZDO once it shows up the next time.";
        }

        node->deleteLater();
    });
}

void ZigbeeNetwork::printNetwork()
{
    qCDebug(dcZigbeeNetwork()) << this;
    foreach (ZigbeeNode *node, nodes()) {
        qCDebug(dcZigbeeNetwork()) << " ---> " << node;
        if (!node->manufacturerName().isEmpty())
            qCDebug(dcZigbeeNetwork()) << "  Manufacturer:" << node->manufacturerName();

        if (!node->modelName().isEmpty())
            qCDebug(dcZigbeeNetwork()) << "  Model:" << node->modelName();

        if (!node->version().isEmpty())
            qCDebug(dcZigbeeNetwork()) << "  Version:" << node->version();

        if (node->nodeDescriptorAvailable()) {
            qCDebug(dcZigbeeNetwork()) << "  " << node->nodeDescriptor();
        } else {
            qCDebug(dcZigbeeNetwork()) << "  Node descriptor not available.";
        }
        if (node->powerDescriptorAvailable()) {
            qCDebug(dcZigbeeNetwork()) << "  " << node->powerDescriptor();
        } else {
            qCDebug(dcZigbeeNetwork()) << "  Power descriptor not available.";
        }
        qCDebug(dcZigbeeNetwork()) << "  Endpoints: " << node->endpoints().count();
        foreach (ZigbeeNodeEndpoint *endpoint, node->endpoints()) {
            qCDebug(dcZigbeeNetwork()) << "    - " << endpoint;
            qCDebug(dcZigbeeNetwork()) << "      Input clusters:";
            foreach (ZigbeeCluster *cluster, endpoint->inputClusters()) {
                qCDebug(dcZigbeeNetwork()) << "      - " << cluster;
                foreach (const ZigbeeClusterAttribute &attribute, cluster->attributes()) {
                    qCDebug(dcZigbeeNetwork()) << "        - " << attribute;
                }
            }
            qCDebug(dcZigbeeNetwork()) << "      Output clusters:";
            foreach (ZigbeeCluster *cluster, endpoint->outputClusters()) {
                qCDebug(dcZigbeeNetwork()) << "      - " << cluster;
                foreach (const ZigbeeClusterAttribute &attribute, cluster->attributes()) {
                    qCDebug(dcZigbeeNetwork()) << "        - " << attribute;
                }
            }
        }
    }
}

void ZigbeeNetwork::addNodeInternally(ZigbeeNode *node)
{
    if (m_nodes.contains(node)) {
        qCWarning(dcZigbeeNetwork()) << "The node" << node << "has already been added.";
        return;
    }

    // Set the coordinator node if the short address is 0x0000
    if (node->shortAddress() == 0) {
        m_coordinatorNode = node;
        m_macAddress = m_coordinatorNode->extendedAddress();
        emit macAddressChanged(m_macAddress);
    }

    // Update database metrics of the node
    connect(node, &ZigbeeNode::lqiChanged, this, [this, node](quint8 lqi){
        m_database->updateNodeLqi(node, lqi);
    });

    connect(node, &ZigbeeNode::lastSeenChanged, this, [this, node](const QDateTime &lastSeen){
        m_database->updateNodeLastSeen(node, lastSeen);
    });

    connect(node, &ZigbeeNode::clusterAdded, this, [this, node](ZigbeeCluster *cluster){
        if (node->state() == ZigbeeNode::StateInitialized) {
            qCWarning(dcZigbeeNetwork()) << node << cluster << "cluster added but the node has already been initialized. This node is out of spec. Save the node nether the less...";
            m_database->saveNode(node);
        }
    });

    // Note: if a cluster shows up after initialization (out of spec devices), save the cluster and it's attributes
    foreach (ZigbeeNodeEndpoint *endpoint, node->endpoints()) {
        connect(endpoint, &ZigbeeNodeEndpoint::clusterAttributeChanged, this, &ZigbeeNetwork::onNodeClusterAttributeChanged);
    }

    m_nodes.append(node);
    emit nodeAdded(node);
}

void ZigbeeNetwork::removeNodeInternally(ZigbeeNode *node)
{
    if (!m_nodes.contains(node)) {
        qCWarning(dcZigbeeNetwork()) << "Try to remove node" << node << "but not in the node list.";
        return;
    }

    if (node == m_coordinatorNode) {
        m_coordinatorNode = nullptr;
    }

    m_nodes.removeAll(node);
    emit nodeRemoved(node);

    m_database->removeNode(node);
}

void ZigbeeNetwork::initializeDatabase()
{
    if (!m_database) {
        QString networkDatabaseFileName = settingsDirectory().absolutePath() + QDir::separator() + QString("zigbee-network-%1.db").arg(networkUuid().toString().remove('{').remove('}'));
        qCDebug(dcZigbeeNetwork()) << "Using ZigBee network database" << QFileInfo(networkDatabaseFileName).fileName();
        m_database = new ZigbeeNetworkDatabase(this, networkDatabaseFileName, this);
    }
}

ZigbeeNode *ZigbeeNetwork::createNode(quint16 shortAddress, const ZigbeeAddress &extendedAddress, QObject *parent)
{
    return new ZigbeeNode(this, shortAddress, extendedAddress, parent);
}

ZigbeeNode *ZigbeeNetwork::createNode(quint16 shortAddress, const ZigbeeAddress &extendedAddress, quint8 macCapabilities, QObject *parent)
{
    ZigbeeNode *node = createNode(shortAddress, extendedAddress, parent);
    node->m_macCapabilities = ZigbeeDeviceProfile::parseMacCapabilities(macCapabilities);
    return node;
}

void ZigbeeNetwork::evaluateNextNodeReachableState()
{
    if (m_reachableRefreshAddresses.isEmpty())
        return;

    ZigbeeNode *node = getZigbeeNode(m_reachableRefreshAddresses.takeFirst());
    if (!node) {
        // Not does not exit any more...continue
        evaluateNextNodeReachableState();
        return;
    }

    // Make a lqi request in order to check if the node is reachable
    ZigbeeDeviceObjectReply *zdoReply = node->deviceObject()->requestNetworkAddress();
    connect(zdoReply, &ZigbeeDeviceObjectReply::finished, this, [=](){
        if (zdoReply->error()) {
            qCWarning(dcZigbeeNetwork()) << node << "seems not to be reachable" << zdoReply->error();
            setNodeReachable(node, false);
        } else {
            setNodeReachable(node, true);
        }

        // Give some time for other requests to be processed
        QTimer::singleShot(5000, this, &ZigbeeNetwork::evaluateNextNodeReachableState);
    });
}

void ZigbeeNetwork::loadNetwork()
{
    if (m_networkLoaded) {
        qCDebug(dcZigbeeNetwork()) << "Network already loaded";
        return;
    }

    qCDebug(dcZigbeeNetwork()) << "Loading network from settings directory" << m_settingsDirectory.absolutePath();
    if (!m_database) {
        QString networkDatabaseFileName = m_settingsDirectory.absolutePath() + QDir::separator() + QString("zigbee-network-%1.db").arg(m_networkUuid.toString().remove('{').remove('}'));
        qCDebug(dcZigbeeNetwork()) << "Using ZigBee network database" << QFileInfo(networkDatabaseFileName).fileName();
        m_database = new ZigbeeNetworkDatabase(this, networkDatabaseFileName, this);
    }

    QList<ZigbeeNode *> nodes = m_database->loadNodes();
    foreach (ZigbeeNode *node, nodes) {
        node->setState(ZigbeeNode::StateInitialized);
        addNodeInternally(node);
    }

    m_networkLoaded = true;
}

void ZigbeeNetwork::clearSettings()
{
    // Note: this clears the database
    qCDebug(dcZigbeeNetwork()) << "Remove zigbee nodes from network";
    foreach (ZigbeeNode *node, m_nodes) {
        removeNode(node);
    }

    qCDebug(dcZigbeeNetwork()) << "Clear all uninitialized nodes";
    foreach (ZigbeeNode *node, m_uninitializedNodes) {
        qCDebug(dcZigbeeNetwork()) << "Remove uninitialized" << node;
        m_uninitializedNodes.removeAll(node);
        node->deleteLater();
    }

    qCDebug(dcZigbeeNetwork()) << "Delete network database";
    if (m_database) {
        if (!m_database->wipeDatabase()) {
            qCWarning(dcZigbeeNetwork()) << "Failed to wipe the network database" << m_database->databaseName();
        }
        delete m_database;
        m_database = nullptr;
    }

    // Reset network configurations
    qCDebug(dcZigbeeNetwork()) << "Clear network properties";
    m_networkLoaded = false;
    setExtendedPanId(0);
    setChannel(0);
    setSecurityConfiguration(ZigbeeSecurityConfiguration());
    setState(StateUninitialized);
    setPermitJoiningEnabled(false);
    m_nodeType = ZigbeeDeviceProfile::NodeTypeCoordinator;
}

bool ZigbeeNetwork::hasUninitializedNode(const ZigbeeAddress &address) const
{
    foreach (ZigbeeNode *node, m_uninitializedNodes) {
        if (node->extendedAddress() == address) {
            return true;
        }
    }

    return false;
}

void ZigbeeNetwork::addNode(ZigbeeNode *node)
{
    qCDebug(dcZigbeeNetwork()) << "Add node" << node;
    if (hasNode(node->extendedAddress())) {
        qCWarning(dcZigbeeNetwork()) << "Not adding node to the system since already added" << node;
        return;
    }

    m_database->saveNode(node);
    addNodeInternally(node);
}

void ZigbeeNetwork::addUnitializedNode(ZigbeeNode *node)
{
    if (m_uninitializedNodes.contains(node)) {
        qCWarning(dcZigbeeNetwork()) << "The uninitialized node" << node << "has already been added.";
        return;
    }

    connect(node, &ZigbeeNode::stateChanged, this, &ZigbeeNetwork::onNodeStateChanged);
    connect(node, &ZigbeeNode::nodeInitializationFailed, this, [this, node](){
        qCWarning(dcZigbeeNetwork()) << "The initialization procedure for" << node << "failed. Please retry to add this node by restarting the init procedure.";
        m_uninitializedNodes.removeAll(node);
        node->deleteLater();
    });

    m_uninitializedNodes.append(node);
    emit nodeJoined(node);
}

void ZigbeeNetwork::removeNode(ZigbeeNode *node)
{
    qCDebug(dcZigbeeNetwork()) << "Remove node" << node;
    removeNodeInternally(node);
    node->deleteLater();
}

void ZigbeeNetwork::removeUninitializedNode(ZigbeeNode *node)
{
    qCDebug(dcZigbeeNetwork()) << "Remove uninitialized node" << node;
    m_uninitializedNodes.removeAll(node);
    node->deleteLater();
}

void ZigbeeNetwork::setNodeReachable(ZigbeeNode *node, bool reachable)
{
    node->setReachable(reachable);
}

void ZigbeeNetwork::updateReplyRequest(ZigbeeNetworkReply *reply, const ZigbeeNetworkRequest &request)
{
    reply->m_request = request;
}

void ZigbeeNetwork::setNodeInformation(ZigbeeNode *node, const QString &manufacturerName, const QString &modelName, const QString &version)
{
    node->m_manufacturerName = manufacturerName;
    emit node->manufacturerNameChanged(node->manufacturerName());

    node->m_modelName = modelName;
    emit node->modelNameChanged(node->modelName());

    node->m_version = version;
    emit node->versionChanged(node->version());

    m_database->saveNode(m_coordinatorNode);
}

void ZigbeeNetwork::setState(ZigbeeNetwork::State state)
{
    if (m_state == state)
        return;

    qCDebug(dcZigbeeNetwork()) << "State changed" << state;
    m_state = state;

    if (state == StateRunning) {
        printNetwork();
    }
    emit stateChanged(m_state);
}

void ZigbeeNetwork::setError(ZigbeeNetwork::Error error)
{
    if (m_error == error)
        return;

    if (m_error != ErrorNoError) qCDebug(dcZigbeeNetwork()) << "Error occured" << error;
    m_error = error;
    emit errorOccured(m_error);
}

bool ZigbeeNetwork::networkConfigurationAvailable() const
{
    return m_extendedPanId != 0 && m_channel != 0 && m_coordinatorNode;
}

void ZigbeeNetwork::handleNodeIndication(ZigbeeNode *node, const Zigbee::ApsdeDataIndication indication)
{
    node->handleDataIndication(indication);
}

void ZigbeeNetwork::handleZigbeeDeviceProfileIndication(const Zigbee::ApsdeDataIndication &indication)
{
    // Check if this is a device announcement
    if (indication.clusterId == ZigbeeDeviceProfile::DeviceAnnounce) {
        QDataStream stream(indication.asdu);
        stream.setByteOrder(QDataStream::LittleEndian);
        quint8 sequenceNumber = 0; quint16 shortAddress = 0; quint64 ieeeAddress = 0; quint8 macFlag = 0;
        stream >> sequenceNumber >> shortAddress >> ieeeAddress >> macFlag;
        onDeviceAnnounced(shortAddress, ZigbeeAddress(ieeeAddress), macFlag);
        return;
    }

    if (indication.destinationShortAddress == Zigbee::BroadcastAddressAllNodes ||
            indication.destinationShortAddress == Zigbee::BroadcastAddressAllRouters ||
            indication.destinationShortAddress == Zigbee::BroadcastAddressAllNonSleepingNodes) {
        qCDebug(dcZigbeeNetwork()) << "Received unhandled broadcast ZDO indication" << indication;

        // FIXME: check what we can do with such messages like permit join
        return;
    }

    // Check if we have a node, uninitalized node or temporary node
    ZigbeeNode *node = getZigbeeNode(indication.sourceShortAddress);
    if (!node) {
        qCWarning(dcZigbeeNetwork()) << "Received a ZDO indication for an unrecognized node. There is no such node in the system. Ignoring indication" << indication;
        // Maybe the network address has changed due to parent node switching. Lets fetch the IEEE address and check again if know this node.
        // If the node is known, the network address gets updated, otherwise the leave network command will be sent.
        verifyUnrecognizedNode(indication.sourceShortAddress);
        return;
    }

    // Let the node handle this indication
    handleNodeIndication(node, indication);
}

void ZigbeeNetwork::handleZigbeeClusterLibraryIndication(const Zigbee::ApsdeDataIndication &indication)
{
    ZigbeeClusterLibrary::Frame frame = ZigbeeClusterLibrary::parseFrameData(indication.asdu);
    //qCDebug(dcZigbeeNetwork()) << "Handle ZCL indication" << indication << frame;

    // Get the node
    ZigbeeNode *node = getZigbeeNode(indication.sourceShortAddress);
    if (!node) {
        qCWarning(dcZigbeeNetwork()) << "Received a ZCL indication from an unrecognized node. Checking IEEE address for this node" << indication;

        // Maybe the network address has changed due to parent node switching. Lets fetch the IEEE address and check again if know this node.
        // If the node is known, the network address gets updated, otherwise the leave network command will be sent.

        verifyUnrecognizedNode(indication.sourceShortAddress);
        return;
    }

    // Let the node handle this indication
    handleNodeIndication(node, indication);
}

void ZigbeeNetwork::onDeviceAnnounced(quint16 shortAddress, ZigbeeAddress ieeeAddress, quint8 macCapabilities)
{
    qCDebug(dcZigbeeNetwork()) << "Device announced" << ZigbeeUtils::convertUint16ToHexString(shortAddress) << ieeeAddress.toString() << ZigbeeUtils::convertByteToHexString(macCapabilities);

    // Lets check if this device is in the uninitialized node list, if so, remove it and recreate the device
    if (hasUninitializedNode(ieeeAddress)) {
        qCWarning(dcZigbeeNetwork()) << "Device announced but there is already an initialization running for it. Remove the device and restart the initialization.";
        ZigbeeNode *uninitializedNode = getZigbeeNode(ieeeAddress);
        removeUninitializedNode(uninitializedNode);
    }

    if (hasNode(ieeeAddress)) {
        ZigbeeNode *node = getZigbeeNode(ieeeAddress);
        if (shortAddress == node->shortAddress()) {
            qCDebug(dcZigbeeNetwork()) << "Already known device announced and is reachable again" << node;
            setNodeReachable(node, true);
            return;
        } else {
            qCDebug(dcZigbeeNetwork()) << "Already known device announced with different network address. Updating the network address internally of this node...";
            updateNodeNetworkAddress(node, shortAddress);
            return;
        }
    }

    ZigbeeNode *node = createNode(shortAddress, ieeeAddress, macCapabilities, this);
    addUnitializedNode(node);
    node->startInitialization();
}

void ZigbeeNetwork::verifyUnrecognizedNode(quint16 shortAddress)
{
    // 1. Create a temporary node for message handling
    // 2. Get the IEEE address
    // 3. Check if we have a node for this address
    //    Yes -> update the network address and save database
    //    No -> send management leave request to the node

    ZigbeeNode *node = new ZigbeeNode(this, shortAddress, ZigbeeAddress(), this);
    m_temporaryNodes.append(node);

    qCDebug(dcZigbeeNetwork()) << "Start verify process for unrecognized node" << node;
    qCDebug(dcZigbeeNetwork()) << "Request IEEE address from unrecognized node" << node;
    ZigbeeDeviceObjectReply *zdoReply = node->deviceObject()->requestIeeeAddress();
    connect(zdoReply, &ZigbeeDeviceObjectReply::finished, node, [=](){
        if (zdoReply->error() != ZigbeeDeviceObjectReply::ErrorNoError) {
            qCWarning(dcZigbeeNode()) << "Failed to request IEEE address from unrecognized" << node << zdoReply->error();
            // Remove and delete this temporary node since we did not know the IEEE address
            qCDebug(dcZigbeeNetwork()) << "Request unrecognized" << node << "to leave the newtork";
            ZigbeeDeviceObjectReply *zdoReply = node->deviceObject()->requestMgmtLeaveNetwork();
            connect(zdoReply, &ZigbeeDeviceObjectReply::finished, node, [=](){
                if (zdoReply->error() != ZigbeeDeviceObjectReply::ErrorNoError) {
                    qCWarning(dcZigbeeNode()) << "Failed to request unrecognized node to leave the network" << node << zdoReply->error();
                    m_temporaryNodes.removeAll(node);
                    node->deleteLater();
                    return;
                }

                qCDebug(dcZigbeeNetwork()) << "Removed unrecognized node successfully from the network" << node;
                m_temporaryNodes.removeAll(node);
                node->deleteLater();
            });

            return;
        }

        QByteArray response = zdoReply->responseData();
        QDataStream stream(&response, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        quint8 sqn; quint8 statusInt; quint64 ieeeAddressInt; quint16 nwkAddress;
        stream >> sqn >> statusInt >> ieeeAddressInt >> nwkAddress;
        ZigbeeDeviceProfile::Status status = static_cast<ZigbeeDeviceProfile::Status>(statusInt);
        ZigbeeAddress ieeeAddress(ieeeAddressInt);
        node->m_extendedAddress = ieeeAddress;
        qCDebug(dcZigbeeDeviceObject()) << "Get IEEE address from unrecognized node finished" << status << ieeeAddress.toString() << ZigbeeUtils::convertUint16ToHexString(nwkAddress) << ZigbeeUtils::convertByteArrayToHexString(zdoReply->responseData());

        if (hasNode(ieeeAddress)) {
            // We know this node with this IEEE address, let's update the network address and save the new address in the database
            qCDebug(dcZigbeeNetwork()) << "Found node for unrecognized network address with IEEE address" << ieeeAddress.toString() << "Updating the network address internally...";
            m_temporaryNodes.removeAll(node);
            node->deleteLater();

            ZigbeeNode *existingNode = getZigbeeNode(ieeeAddress);
            updateNodeNetworkAddress(existingNode, shortAddress);
            return;
        } else {
            // We don't know any node with this ieeeAddress. Let's try to make it leave the network
            qCWarning(dcZigbeeNetwork()) << "Could not find any node with IEEE address" << ieeeAddress.toString() << "Requesting node to leave the network" << ZigbeeUtils::convertUint16ToHexString(shortAddress);

            qCDebug(dcZigbeeNetwork()) << "Request unrecognized" << node << "to leave the newtork";
            ZigbeeDeviceObjectReply *zdoReply = node->deviceObject()->requestMgmtLeaveNetwork();
            connect(zdoReply, &ZigbeeDeviceObjectReply::finished, node, [=](){
                if (zdoReply->error() != ZigbeeDeviceObjectReply::ErrorNoError) {
                    qCWarning(dcZigbeeNode()) << "Failed to request unrecognized node to leave the network" << node << zdoReply->error();
                    m_temporaryNodes.removeAll(node);
                    node->deleteLater();
                    return;
                }

                qCDebug(dcZigbeeNetwork()) << "Removed unrecognized node successfully from the network" << node;
                m_temporaryNodes.removeAll(node);
                node->deleteLater();
            });
        }
    });
}

void ZigbeeNetwork::updateNodeNetworkAddress(ZigbeeNode *node, quint16 shortAddress)
{
    qCDebug(dcZigbeeNetwork()) << "Network address of" << node << "has changed to" << ZigbeeUtils::convertUint16ToHexString(shortAddress);
    node->m_shortAddress = shortAddress;
    emit node->shortAddressChanged(shortAddress);

    m_database->updateNodeNetworkAddress(node, shortAddress);
    setNodeReachable(node, true);
}

ZigbeeNetworkReply *ZigbeeNetwork::createNetworkReply(const ZigbeeNetworkRequest &request)
{
    ZigbeeNetworkReply *reply = new ZigbeeNetworkReply(request, this);
    // Make sure the reply will be deleted
    connect(reply, &ZigbeeNetworkReply::finished, reply, &ZigbeeNetworkReply::deleteLater, Qt::QueuedConnection);
    return reply;
}

void ZigbeeNetwork::setReplyResponseError(ZigbeeNetworkReply *reply, quint8 zigbeeStatus)
{
    if (zigbeeStatus == Zigbee::ZigbeeApsStatusSuccess) {
        // The request has been sent successfully to the device
        finishNetworkReply(reply);
    } else {
        // There has been an error while transporting the request to the device
        if (zigbeeStatus >= 0xc1 && zigbeeStatus <= 0xd4) {
            reply->m_zigbeeNwkStatus = static_cast<Zigbee::ZigbeeNwkLayerStatus>(static_cast<quint8>(zigbeeStatus));
            finishNetworkReply(reply, ZigbeeNetworkReply::ErrorZigbeeNwkStatusError);
        } else if (zigbeeStatus >= 0xE0 && zigbeeStatus <= 0xF4) {
            reply->m_zigbeeMacStatus = static_cast<Zigbee::ZigbeeMacLayerStatus>(static_cast<quint8>(zigbeeStatus));
            finishNetworkReply(reply, ZigbeeNetworkReply::ErrorZigbeeMacStatusError);
        } else {
            reply->m_zigbeeApsStatus = static_cast<Zigbee::ZigbeeApsStatus>(zigbeeStatus);
            finishNetworkReply(reply, ZigbeeNetworkReply::ErrorZigbeeApsStatusError);
        }
    }
}

void ZigbeeNetwork::finishNetworkReply(ZigbeeNetworkReply *reply, ZigbeeNetworkReply::Error error)
{
    qCDebug(dcZigbeeNetwork()) << "Finish network reply" << reply << error;
    reply->m_error = error;
    switch(reply->error()) {
    case ZigbeeNetworkReply::ErrorNoError:
        qCDebug(dcZigbeeNetwork()) << "Network request sent successfully to device" << reply->request();
        break;
    case ZigbeeNetworkReply::ErrorZigbeeApsStatusError:
        qCWarning(dcZigbeeNetwork()) << "Failed to send request to device" << reply->request() << reply->error() << reply->zigbeeApsStatus();
        break;
    case ZigbeeNetworkReply::ErrorZigbeeNwkStatusError:
        qCWarning(dcZigbeeNetwork()) << "Failed to send request to device" << reply->request() << reply->error() << reply->zigbeeNwkStatus();
        break;
    case ZigbeeNetworkReply::ErrorZigbeeMacStatusError:
        qCWarning(dcZigbeeNetwork()) << "Failed to send request to device" << reply->request() << reply->error() << reply->zigbeeMacStatus();
        break;
    default:
        qCWarning(dcZigbeeNetwork()) << "Failed to send request to device" << reply->request() << reply->error();
        break;
    }

    // Stop the timer
    reply->m_timer->stop();

    // Finish the reply
    reply->finished();
}

void ZigbeeNetwork::startWaitingReply(ZigbeeNetworkReply *reply)
{
    reply->m_timer->start();
}

void ZigbeeNetwork::onNodeStateChanged(ZigbeeNode::State state)
{
    ZigbeeNode *node = qobject_cast<ZigbeeNode *>(sender());
    if (state == ZigbeeNode::StateInitialized && m_uninitializedNodes.contains(node)) {
        m_uninitializedNodes.removeAll(node);
        // Disconnect this slot since we don't need it any more
        disconnect(node, &ZigbeeNode::stateChanged, this, &ZigbeeNetwork::onNodeStateChanged);
        addNode(node);
    }
}

void ZigbeeNetwork::onNodeClusterAttributeChanged(ZigbeeCluster *cluster, const ZigbeeClusterAttribute &attribute)
{
    m_database->saveAttribute(cluster, attribute);
}

void ZigbeeNetwork::evaluateNodeReachableStates()
{
    qCDebug(dcZigbeeNetwork()) << "Evaluate reachable state of nodes";
    m_reachableRefreshAddresses.clear();

    foreach (ZigbeeNode *node, m_nodes) {
        // Skip the coordinator
        if (node->shortAddress() == 0x0000)
            continue;

        if (node->macCapabilities().receiverOnWhenIdle && node->shortAddress() != 0x0000) {

            // Lets send a request to all things which are not reachable
            if (!node->reachable()) {
                qCDebug(dcZigbeeNetwork()) << node << "enqueue evaluating reachable state";
                m_reachableRefreshAddresses.append(node->extendedAddress());
                continue;
            }

            // Lets send a request to nodes which have not been seen more than 10 min
            int msSinceLastSeen = node->lastSeen().msecsTo(QDateTime::currentDateTimeUtc());
            qCDebug(dcZigbeeNetwork()) << node << "has been seen the last time" << QTime::fromMSecsSinceStartOfDay(msSinceLastSeen).toString() << "ago.";
            // 10 min = 10 * 60 * 1000 = 600000 ms
            if (msSinceLastSeen > 600000) {
                qCDebug(dcZigbeeNetwork()) << node << "enqueue evaluating reachable state";
                m_reachableRefreshAddresses.append(node->extendedAddress());
            }
        } else {
            // Note: sleeping devices should send some message within 6 hours,
            // otherwise the device might not be reachable any more
            int msSinceLastSeen = node->lastSeen().msecsTo(QDateTime::currentDateTimeUtc());
            qCDebug(dcZigbeeNetwork()) << node << "has been seen the last time" << QTime::fromMSecsSinceStartOfDay(msSinceLastSeen).toString() << "ago.";
            // 6 Hours = 6 * 60 * 60 * 1000 = 21600000 ms
            if (msSinceLastSeen < 21600000) {
                setNodeReachable(node, true);
            } else {
                setNodeReachable(node, false);
            }
        }
    }

    evaluateNextNodeReachableState();
}

QDebug operator<<(QDebug debug, ZigbeeNetwork *network)
{
    debug.nospace().noquote() << "ZigbeeNetwork(" << network->macAddress().toString() << ", "
                              << network->networkUuid().toString() << ", "
                              << network->backendType() << ", "
                              << "Channel: " << network->channel() << ", "
                              << network->state()
                              << ")";
    return debug.space().quote();
}
