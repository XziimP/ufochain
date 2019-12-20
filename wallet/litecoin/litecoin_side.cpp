// Copyright 2019 The UFO Team
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

#include "litecoin_side.h"
#include "common.h"

namespace
{
    constexpr uint32_t kLitecoinWithdrawTxAverageSize = 360; 
    constexpr uint32_t kLitecoinLockTxEstimatedTimeInUfoBlocks = 20;   // it's average value
}

namespace ufo::wallet
{
    LitecoinSide::LitecoinSide(BaseTransaction& tx, bitcoin::IBridge::Ptr bitcoinBridge, litecoin::ISettingsProvider& settingsProvider, bool isUfoSide)
        : BitcoinSide(tx, bitcoinBridge, settingsProvider, isUfoSide)
    {
    }

    bool LitecoinSide::CheckAmount(Amount amount, Amount feeRate)
    {
        //TODO:double?
        Amount fee = static_cast<Amount>(std::round(double(kLitecoinWithdrawTxAverageSize * feeRate) / 1000));
        return amount > litecoin::kDustThreshold && amount > fee;
    }

    uint32_t LitecoinSide::GetLockTxEstimatedTimeInUfoBlocks() const
    {
        return kLitecoinLockTxEstimatedTimeInUfoBlocks;
    }

    uint8_t LitecoinSide::GetAddressVersion() const
    {
        return litecoin::getAddressVersion();
    }
}