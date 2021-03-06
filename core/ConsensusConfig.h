/**
 *  Copyright (C) 2021 FISCO BCOS.
 *  SPDX-License-Identifier: Apache-2.0
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * @brief implementation of Consensus Config
 * @file ConsensusConfig.h
 * @author: yujiechen
 * @date 2021-04-09
 */
#pragma once
#include "Common.h"
#include "framework/ConsensusConfigInterface.h"
#include <bcos-framework/interfaces/crypto/KeyPairInterface.h>
#include <bcos-framework/libutilities/Common.h>

namespace bcos
{
namespace consensus
{
class ConsensusConfig : public ConsensusConfigInterface
{
public:
    using Ptr = std::shared_ptr<ConsensusConfig>;
    explicit ConsensusConfig(bcos::crypto::KeyPairInterface::Ptr _keyPair)
      : m_keyPair(_keyPair), m_consensusNodeList(std::make_shared<ConsensusNodeList>())
    {}
    virtual ~ConsensusConfig() {}

    // the NodeID of the consensus node
    bcos::crypto::PublicPtr nodeID() const override { return m_keyPair->publicKey(); }

    // the nodeIndex of this node
    IndexType nodeIndex() const override { return m_nodeIndex; }

    // the consensus node list
    ConsensusNodeList const& consensusNodeList() const override;

    uint64_t consensusTimeout() const override { return m_consensusTimeout; }

    void setConsensusNodeList(ConsensusNodeList& _consensusNodeList) override;

    void setConsensusTimeout(uint64_t _consensusTimeout) override
    {
        m_consensusTimeout.store(_consensusTimeout);
    }

    void setCommittedProposal(ProposalInterface::Ptr _committedProposal) override
    {
        WriteGuard l(x_committedProposal);
        m_committedProposal = _committedProposal;
    }

    ProposalInterface::ConstPtr committedProposal() override
    {
        ReadGuard l(x_committedProposal);
        return std::const_pointer_cast<ProposalInterface const>(m_committedProposal);
    }
    bcos::crypto::KeyPairInterface::Ptr keyPair() { return m_keyPair; }

    virtual void updateQuorum() = 0;
    IndexType getNodeIndexByNodeID(bcos::crypto::PublicPtr _nodeID);
    ConsensusNodeInterface::Ptr getConsensusNodeByIndex(IndexType _nodeIndex);

protected:
    bcos::crypto::KeyPairInterface::Ptr m_keyPair;
    std::atomic<IndexType> m_nodeIndex;
    std::atomic<IndexType> m_consensusNodeNum;

    ConsensusNodeListPtr m_consensusNodeList;
    mutable bcos::SharedMutex x_consensusNodeList;

    // default timeout is 3000ms
    std::atomic<uint64_t> m_consensusTimeout = {3000};

    ProposalInterface::Ptr m_committedProposal;
    mutable bcos::SharedMutex x_committedProposal;
};
}  // namespace consensus
}  // namespace bcos