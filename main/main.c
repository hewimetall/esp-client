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

