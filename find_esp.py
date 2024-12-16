# find_esp.py
import platform
import socket
from SCons.Script import ARGUMENTS, DefaultEnvironment

def get_network_prefix():
    # Get local IP to determine network range
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('8.8.8.8', 1))
        local_ip = s.getsockname()[0]
    except Exception:
        local_ip = '127.0.0.1'
    finally:
        s.close()
    
    return '.'.join(local_ip.split('.')[:-1])

def scan_network():
    network_prefix = get_network_prefix()
    esp_ports = []
    
    print(f"Scanning network {network_prefix}.0/24 for ESP devices...")
    
    for i in range(1, 255):
        ip = f"{network_prefix}.{i}"
        
        # Try to connect to ESP OTA port (default 8266 for ESP8266)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(0.1)
        
        try:
            # Changed port to 8266 for ESP8266
            if sock.connect_ex((ip, 8266)) == 0:
                print(f"Found potential ESP device at {ip}")
                esp_ports.append(ip)
        except:
            pass
        finally:
            sock.close()
    
    return esp_ports

def env_override_options(env):
    esp_devices = scan_network()
    
    if not esp_devices:
        print("No ESP devices found on network!")
        env.Exit(1)  # Exit if no devices found
    
    if len(esp_devices) == 1:
        print(f"Found single ESP device at {esp_devices[0]}")
        env['UPLOAD_PORT'] = esp_devices[0]  # Changed to dictionary style access
    else:
        print("\nMultiple ESP devices found:")
        for i, ip in enumerate(esp_devices):
            print(f"{i+1}. {ip}")
        
        selection = input("\nSelect device number to upload to: ")
        try:
            selected_ip = esp_devices[int(selection) - 1]
            env['UPLOAD_PORT'] = selected_ip  # Changed to dictionary style access
        except:
            print("Invalid selection, aborting.")
            env.Exit(1)

# Make the function available to PlatformIO
env = DefaultEnvironment()
env.AddPreAction("upload", env_override_options)