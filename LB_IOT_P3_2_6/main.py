import socket
import time


def my_server():
    # Define host and port
    host = "0.0.0.0"  # Listen on all local network interfaces
    port = 5000  # Port to listen on

    # Create a TCP/IP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Allow immediate reuse of the port
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind the socket to the address and port
    server_socket.bind((host, port))

    # Configure how many clients the server can listen to
    server_socket.listen(2)
    print(f"[+] Server listening on port {port}...")

    # Accept incoming connection
    conn, address = server_socket.accept()
    print(f"[+] Connection from: {str(address)}")

    try:
        # Send start command to ESP32
        time.sleep(1)
        command = "start\n"
        conn.send(command.encode())
        print(f"[>] Sent command: {command.strip()}")

        # Receive data stream
        buffer = ""
        while True:
            # Receive data stream (1024 bytes)
            data = conn.recv(1024).decode('utf-8')
            if not data:
                break

            buffer += data
            while "\n" in buffer:
                line, buffer = buffer.split("\n", 1)
                line = line.strip()

                # Process accelerometer data
                if line.startswith("ACCEL:"):
                    raw_data = line.replace("ACCEL:", "")
                    accel_x, accel_y, accel_z = raw_data.split(",")
                    print(f"Accel Data -> X: {accel_x} m/s² | Y: {accel_y} m/s² | Z: {accel_z} m/s²")
                else:
                    print(f"Server received: {line}")

    except KeyboardInterrupt:
        print("\n[!] Stopping server and sending stop command...")
        stop_cmd = "stop\n"
        conn.send(stop_cmd.encode())
        time.sleep(0.5)

    finally:
        # Close the client and server connection
        conn.close()
        server_socket.close()
        print("[+] Server closed successfully.")


if __name__ == '__main__':
    my_server()