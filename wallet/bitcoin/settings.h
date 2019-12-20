// Copyright 2019 The Ufo Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "core/serialization_adapters.h"
#include "utility/io/address.h"
#include "utility/common.h"
#include "wallet/common.h"
#include "wallet/wallet_db.h"

namespace ufo::bitcoin
{
    struct BitcoinCoreSettings
    {
        std::string m_userName;
        std::string m_pass;
        io::Address m_address;

        std::string generateAuthorization();

        bool IsInitialized() const
        {
            return !m_userName.empty() && !m_pass.empty() && !m_address.empty();
        }

        SERIALIZE(m_userName, m_pass, m_address);
    };

    struct ElectrumSettings
    {
        std::string m_address;
        std::vector<std::string> m_secretWords;
        uint8_t m_addressVersion;

        uint32_t m_receivingAddressAmount = 21;
        uint32_t m_changeAddressAmount = 6;

        bool IsInitialized() const
        {
            return !m_secretWords.empty() && !m_address.empty();
        }

        SERIALIZE(m_address, m_secretWords, m_addressVersion);
    };

    class ISettings
    {
    public:
        using Ptr = std::shared_ptr<ISettings>;

        enum ConnectionType
        {
            None,
            Core,
            Electrum
        };
        
        virtual ~ISettings() {};

        virtual BitcoinCoreSettings GetConnectionOptions() const = 0;
        virtual bool IsCoreActivated() const = 0;
        virtual ElectrumSettings GetElectrumConnectionOptions() const = 0;
        virtual bool IsElectrumActivated() const = 0;
        virtual Amount GetFeeRate() const = 0;
        virtual Amount GetMinFeeRate() const = 0;
        virtual uint16_t GetTxMinConfirmations() const = 0;
        virtual uint32_t GetLockTimeInBlocks() const = 0;
        virtual bool IsInitialized() const = 0;
        virtual bool IsActivated() const = 0;
        virtual ConnectionType GetCurrentConnectionType() const = 0;
    };

    boost::optional<ISettings::ConnectionType> from_string(const std::string&);
    std::string to_string(ISettings::ConnectionType);

    class Settings : public ISettings
    {
    public:
       // Settings() = default;
       // ~Settings() = default;
        BitcoinCoreSettings GetConnectionOptions() const override;
        bool IsCoreActivated() const override;
        ElectrumSettings GetElectrumConnectionOptions() const override;
        bool IsElectrumActivated() const override;
        Amount GetFeeRate() const override;
        Amount GetMinFeeRate() const override;
        uint16_t GetTxMinConfirmations() const override;
        uint32_t GetLockTimeInBlocks() const override;
        bool IsInitialized() const override;
        bool IsActivated() const override;
        ConnectionType GetCurrentConnectionType() const override;

        void SetConnectionOptions(const BitcoinCoreSettings& connectionSettings);
        void SetElectrumConnectionOptions(const ElectrumSettings& connectionSettings);
        void SetFeeRate(Amount feeRate);
        void SetMinFeeRate(Amount feeRate);
        void SetTxMinConfirmations(uint16_t txMinConfirmations);
        void SetLockTimeInBlocks(uint32_t lockTimeInBlocks);
        void ChangeConnectionType(ConnectionType type);

        SERIALIZE(m_connectionSettings, m_electrumConnectionSettings, m_feeRate, m_minFeeRate, m_txMinConfirmations, m_lockTimeInBlocks, m_connectionType);

    protected:
        BitcoinCoreSettings m_connectionSettings;
        ElectrumSettings m_electrumConnectionSettings;
        Amount m_feeRate = 90000;
        Amount m_minFeeRate = 50000;
        uint16_t m_txMinConfirmations = 6;
        // TODO: remove after tests
        uint32_t m_lockTimeInBlocks = 60;
        // TODO: uncomment after tests
        //uint32_t m_lockTimeInBlocks = 2 * 24 * 6;
        ConnectionType m_connectionType = None;
    };
} // namespace ufo::bitcoin