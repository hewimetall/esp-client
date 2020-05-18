#include "head.h"

static const char *TAG = "udp_client";
static const char *payload = "t";
static int sock =0;
static struct sockaddr_in dest_addr;

static esp_err_t socket_start_tx(){

    int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
    	return ESP_FAIL;
    }
	return ESP_OK;
}

static esp_err_t socket_recv(char *rx_buffer,size_t buff_len){
    struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
    socklen_t socklen = sizeof(source_addr);
    int len = recvfrom(sock, rx_buffer, buff_len, 0, (struct sockaddr *)&source_addr, &socklen);

    // Error occurred during receiving
    if (len < 0) {
        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
        return ESP_FAIL;
    }
    // Data received
    else {
        rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
        if (strncmp(rx_buffer, "OK: ", 4) == 0) {
            ESP_LOGI(TAG, "Received expected message, reconnecting");
            return ESP_FAIL;
        }
    }
    return ESP_OK;
}

static void udp_client_task(void *pvParameters)
{
    char rx_buffer[128];
    size_t len =sizeof rx_buffer;
    int addr_family = 0;
    int ip_protocol = 0;
    int arr[2];
    bzero(arr,0);
    while (1) {

        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
    	socket_start_tx();

        while (1) {
        	socket_recv(&rx_buffer,len);
        	json_pars(rx_buffer,sizeof rx_buffer,&arr);
            ESP_LOGI(TAG, "roll :%i pith:%i", arr[0],arr[1]);
            motor_set_state(&arr);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

esp_err_t socket_init(){
	ESP_LOGI(TAG,"socket init");
    xTaskCreate(&udp_client_task, "udp_client", 2048*2, NULL, 5, NULL);
    return ESP_OK;
}
