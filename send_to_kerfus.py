import socket

ESP_IP = "192.168.40.181"
ESP_PORT = 7658

def send_meow():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto("meow".encode(), (ESP_IP, ESP_PORT))

if __name__ == "__main__":
    send_meow()
