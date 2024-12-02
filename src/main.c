/*
 * Copyright (c) 2024 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>

#include <ble_observer.h>
#include <stream_client.h>
#include <usb_net.h>
#include <xiaomi.h>

LOG_MODULE_REGISTER(ble, LOG_LEVEL_INF);

int main(void)
{
	int ret;

	printk("Hello, World!\n");

#if CONFIG_COPRO_USB_NETWORK
	/* Initialize NET interface management */
	usb_net_iface_init();
#endif

	/* Initialize the USB Subsystem */
	ret = usb_enable(NULL);
	if (ret != 0) {
		LOG_ERR("Failed to enable USB");
		return ret;
	}

	LOG_INF("USB initialized %d", 0);

	/* Initialize the Bluetooth Subsystem */
	ret = bt_enable(NULL);
	if (ret != 0) {
		LOG_ERR("Bluetooth init failed (ret %d)", ret);
		return ret;
	}

	LOG_INF("Bluetooth initialized %d", 0);

	/* Start the BLE observer thread */
	ble_observer_start();

#if CONFIG_COPRO_XIAOMI_LYWSD03MMC
	/* Configure the stream client */
	stream_client_channel_add(
		STREAM_CHANNEL_ID_XIAOMI, STREAM_CHANNEL_NAME_XIAOMI, &xiaomi_msgq);
#endif

	/* Start the stream client */
	stream_client_start();

	return 0;
}