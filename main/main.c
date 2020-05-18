/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "head.h"
static const char *TAG = "udp_client";

void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	motor_init_d();

    /*Create  event group */
    s_wifi_event_group = xEventGroupCreate();
    status_group = xEventGroupCreate();
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");

    ESP_ERROR_CHECK(wifi_init_sta());
    /* TASK */
    socket_init();
}

