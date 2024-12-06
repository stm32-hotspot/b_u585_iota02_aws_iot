import socket

def test_echo_server(host='192.168.1.25', port=7):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print(f"Connecting to {host}:{port}")
        s.connect((host, port))
        s.sendall(b'Hello, Echo Server!')
        data = s.recv(1024)
        print(f'Received: {data.decode()}')

if __name__ == "__main__":
    test_echo_server('192.168.1.25')
    # test_echo_server('192.168.1.31')
