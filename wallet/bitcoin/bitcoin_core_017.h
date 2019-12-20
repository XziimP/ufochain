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

#include "bitcoin_core_016.h"

namespace ufo::bitcoin
{
    class BitcoinCore017 : public BitcoinCore016
    {
    public:
        BitcoinCore017() = delete;
        BitcoinCore017(io::Reactor& reactor, IBitcoinCoreSettingsProvider& settingsProvider);

        void signRawTransaction(const std::string& rawTx, std::function<void(const IBridge::Error&, const std::string&, bool)> callback) override;
        void createRawTransaction(
            const std::string& withdrawAddress,
            const std::string& contractTxId,
            Amount amount,
            int outputIndex,
            Timestamp locktime,
            std::function<void(const IBridge::Error&, const std::string&)> callback) override;
    };
} // namespace ufo::bitcoin