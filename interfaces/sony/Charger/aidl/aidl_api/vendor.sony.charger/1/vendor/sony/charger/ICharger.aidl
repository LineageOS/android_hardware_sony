/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.sony.charger;

@VintfStability
interface ICharger
{
    void setChargingEnable(in boolean enabled);
    boolean isChargingEnabled();
    void setChargingLimit(in int limit);
    void checkBatCapacityAndApplyLimit();
}
