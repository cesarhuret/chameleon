import serial
import time
import struct
import argparse
from threading import Thread
from prompt_toolkit import PromptSession
from prompt_toolkit.patch_stdout import patch_stdout

# ----------------------------
# Configuration
# ----------------------------
SERIAL_PORT = "COM5"    # Replace with your HC-06 COM port
BAUD_RATE = 57600
RECONNECT_DELAY = 2     # seconds to wait if Arduino not ready
PACKET_START = 0xAA
MAX_PACKET_PAYLOAD = 32
HANDSHAKE_BYTE = 0x7E
HANDSHAKE_ACK_BYTE = 0xAC

LOG_LEVELS = {
    0: "DEBUG",
    1: "INFO",
    2: "ERROR",
}

CODES = {
    0x0: "SUCCESS",
    0x01: "SETUP_SUCCESS",

    0x20: "PIXY_INITIALISING",
    0x21: "PIXY_NOT_AVAILABLE",
    0x22: "PIXY_INVALID_INDEX",
    0x23: "PIXY_BLOCK_NOT_FOUND",
    0x24: "PIXY_TARGET_NOT_FOUND",
    0xff: "PIXY_RESULT_ERROR",
    0xfe: "PIXY_RESULT_BUSY",
    0xfd: "PIXY_RESULT_CHECKSUM_ERROR",
    0xfc: "PIXY_RESULT_TIMEOUT",
    0xfb: "PIXY_RESULT_BUTTON_OVERRIDE",
    0xfa: "PIXY_RESULT_PROG_CHANGING",



    0x30: "ULTRASONIC_NOT_AVAILABLE",
    0x31: "ULTRASONIC_OUT_OF_RANGE",

    0x09: "HALT"
}

STATES = {
    0: "SEARCHING_FOR_BALL",
    1: "CENTERING_TARGET",
    2: "MOVING_TO_BALL",
    3: "GRAB_CLAW",
    4: "RELEASE_CLAW",
    5: "ROTATE_TO_BASE",
    6: "ROTATE_TO_CENTER",
    7: "MOVING_TO_BASE"
}

# ----------------------------
# Function to establish serial connection
# ----------------------------
def connect_serial(serial_port: str, baud_rate: int):
    while True:
        try:
            ser = serial.Serial(serial_port, baud_rate, timeout=1)
            print(f"[INFO] Connected to {serial_port} @ {baud_rate}")
            return ser
        except serial.SerialException:
            print(f"[WARN] Could not open {serial_port}. Retrying in {RECONNECT_DELAY}s...")
            time.sleep(RECONNECT_DELAY)


def wait_for_handshake_ack(ser, timeout_s: float = 5.0) -> bytes | None:
    deadline = time.time() + timeout_s

    while time.time() < deadline:
        waiting = ser.in_waiting
        if waiting > 0:
            data = ser.read(waiting)
            ack_index = data.find(bytes([HANDSHAKE_ACK_BYTE]))
            if ack_index >= 0:
                # Keep bytes that arrived after ACK so we don't lose packet framing.
                return data[ack_index + 1 :]
        time.sleep(0.05)

    return None


def perform_handshake(ser, timeout_s: float = 5.0, retries: int = 3) -> bytes | None:
    attempts = 0

    while retries <= 0 or attempts < retries:
        attempts += 1
        ser.reset_input_buffer()
        ser.write(bytes([HANDSHAKE_BYTE]))
        ser.flush()

        trailing_bytes = wait_for_handshake_ack(ser, timeout_s=timeout_s)
        if trailing_bytes is not None:
            print("[INFO] Handshake complete. Arduino is ready.")
            return trailing_bytes

        print(f"[WARN] Handshake timeout (attempt {attempts}/{retries if retries > 0 else 'inf'}). Retrying...")
        time.sleep(0.5)

    return None


def decode_payload(packet_type: int, payload: bytes) -> str:
    if not payload:
        return "-"
    
    if packet_type == 0x0 and len(payload) == 1:
        code = payload[0]
        return CODES.get(code, f"{code:02X}")

    if packet_type == 3 and len(payload) == 10:  # PackedBlock
        x, y, w, h, sig, age = struct.unpack("<HHHHBB", payload)
        return f"x={x} y={y} w={w} h={h} sig={sig} age={age}"
    
    if packet_type == 2 and len(payload) == 1:  # State code
        state = payload[0]
        return STATES.get(state, f"S{state}")

    # For AVR ints (most of your current log calls), 2-byte signed is common.
    if len(payload) == 2:
        value = struct.unpack("<h", payload)[0]
        return str(value)

    if len(payload) == 1:
        return str(payload[0])

    if len(payload) == 4:
        signed = struct.unpack("<i", payload)[0]
        unsigned = struct.unpack("<I", payload)[0]
        return f"s32={signed} u32={unsigned}"

    return payload.hex(" ")

# ----------------------------
# Function to continuously read serial logs
# ----------------------------
def read_serial(ser, show_raw=False, initial_bytes: bytes = b""):
    rx_buffer = bytearray(initial_bytes)
    last_rx_at = time.time()
    last_status_at = time.time()

    while True:
        try:
            chunk = ser.read(ser.in_waiting or 1)
            if not chunk:
                now = time.time()
                if now - last_status_at >= 5:
                    # print("\r [INFO] Waiting for packets...")
                    last_status_at = now
                continue

            last_rx_at = time.time()
            rx_buffer.extend(chunk)

            while True:
                start_index = rx_buffer.find(PACKET_START)
                if start_index < 0:
                    if show_raw and rx_buffer:
                        preview = bytes(rx_buffer[:32]).hex(" ")
                        print(f"\r [RAW] no-start ({len(rx_buffer)} bytes): {preview}")
                    rx_buffer.clear()
                    break

                if start_index > 0:
                    del rx_buffer[:start_index]

                if len(rx_buffer) < 4:
                    break

                level = rx_buffer[1]
                packet_type = rx_buffer[2]
                size = rx_buffer[3]

                if size > MAX_PACKET_PAYLOAD:
                    del rx_buffer[0]
                    continue

                total_len = 4 + size
                if len(rx_buffer) < total_len:
                    break

                payload = bytes(rx_buffer[4:total_len])
                del rx_buffer[:total_len]

                level_name = LOG_LEVELS.get(level, f"L{level}")
                decoded_payload = decode_payload(packet_type, payload)
                timestamp = time.strftime("%H:%M:%S")
                print(f"[LOG] {timestamp} {level_name} {packet_type} size={size} payload={payload.hex(' ')} decoded={decoded_payload}")

            now = time.time()
            if now - last_status_at >= 5 and now - last_rx_at < 5:
                print("\r [INFO] Receiving serial bytes")
                last_status_at = now
        except Exception as e:
            print(f"[ERROR] Serial read: {e}")
            break


def parse_args():
    parser = argparse.ArgumentParser(description="Bluetooth packet CLI for Arduino logger")
    parser.add_argument("--port", default=SERIAL_PORT, help="Serial COM port (e.g. COM5)")
    parser.add_argument("--baud", type=int, default=BAUD_RATE, help="UART baud rate")
    parser.add_argument("--raw", action="store_true", help="Print raw bytes when no packet start is found")
    parser.add_argument("--skip-handshake", action="store_true", help="Skip startup handshake (use for older firmware or already-running sessions)")
    parser.add_argument("--handshake-timeout", type=float, default=5.0, help="Seconds to wait for each handshake attempt")
    parser.add_argument("--handshake-retries", type=int, default=3, help="Number of handshake attempts; <=0 means infinite")
    return parser.parse_args()

# ----------------------------
# Main program
# ----------------------------
def main():
    args = parse_args()

    # Attempt to connect to HC-06
    ser = connect_serial(args.port, args.baud)

    # Unblock Arduino init() when it is waiting for Bluetooth handshake.
    initial_bytes = b""
    if args.skip_handshake:
        print("[INFO] Handshake skipped by flag.")
    else:
        trailing = perform_handshake(ser, timeout_s=args.handshake_timeout, retries=args.handshake_retries)
        if trailing is None:
            print("[WARN] Handshake failed; continuing anyway. Use --skip-handshake if this is expected.")
        else:
            initial_bytes = trailing

    # Start serial reading in a separate thread
    thread = Thread(target=read_serial, args=(ser, args.raw, initial_bytes), daemon=True)
    thread.start()

    # Start prompt session for user input
    session = PromptSession("> ")

    # Patch stdout so printed logs don’t break the prompt
    with patch_stdout():
        while True:
            try:
                cmd = session.prompt("> ")
                if cmd.strip() == "":
                    continue  # ignore empty commands
                ser.write((cmd + "\n").encode())
            except (KeyboardInterrupt, EOFError):
                print("\nExiting CLI...")
                break

if __name__ == "__main__":
    main()