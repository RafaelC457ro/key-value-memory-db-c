import socket
import sys
import threading
import random
import string
import time

def generate_random_string(length):
    letters = string.ascii_letters
    return ''.join(random.choice(letters) for _ in range(length))

def run_test(host, port, client_id, success_flag):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
            client_socket.connect((host, port))
            
            random_key = generate_random_string(8)
            random_value = generate_random_string(16)
            
            commands = [
                ("set", random_key, random_value),
                ("set", random_key, random_value),  # the random key is causing collisions, so we set it again
                ("get", random_key),
                ("delete", random_key),
                ("get", random_key),
            ]
            
            expected_responses = [
                "OK",
                "OK",
                f"\"{random_value}\"",
                "OK",
                "(nil)",
            ]
            
            for i, command in enumerate(commands):
                full_command = " ".join(command) + "\n"
                client_socket.sendall(full_command.encode())
                response = client_socket.recv(1024).decode().strip()
                expected_response = expected_responses[i]
                
                if response != expected_response:
                    success_flag[client_id] = False  
                
                    print(f"Command sent: {full_command[:-1]}")
                    print(f"Expected response: {expected_response}")
                    print(f"Client {client_id} received incorrect response: {response}")
    
    except Exception as e:
        print(f"Client {client_id}: An error occurred: {e}")
        success_flag[client_id] = False

    finally:
        client_socket.close()


def main():
    host= "127.0.0.1"
    port = 8080
    num_connections = 1024
    success_flag = [True] * num_connections  
    threads = []
    start_time = time.time() 
    
    threads = []
    for i in range(num_connections):
        thread = threading.Thread(target=run_test, args=(host, port, i,success_flag))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    end_time = time.time()  
    elapsed_time = end_time - start_time
    print(f"Total elapsed time for all threads: {elapsed_time:.4f} seconds")

    success_count = sum(filter(lambda x: x, success_flag))

    if all(success_flag):
        print(f"All {num_connections} threads ran successfully.")
        sys.exit(0)  
    else:
        print(f"{success_count} out of {num_connections} threads ran successfully.")
        sys.exit(1)  

   
if __name__ == "__main__":
    main()