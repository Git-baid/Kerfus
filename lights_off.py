import socket

KERFUS_IP = "192.168.40.181"
SWITCHER_IP = "192.168.40.183"
ESP_PORT = 7658

def send_meow():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto("meow".encode(), (KERFUS_IP, ESP_PORT))

def lights_off():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto("lights_off".encode(), (SWITCHER_IP, ESP_PORT))

if __name__ == "__main__":
    send_meow()
    lights_off()
