def xor_distance(node_id_a: str, node_id_b: str) -> int:
    """
    Calculates the XOR distance between two node IDs.
    Parameters node_id_a and node_id_b are binary strings, e.g., '000000000000...00110000'
    Returns the XOR distance as an integer.
    """
    int_a = int(node_id_a, 2)
    int_b = int(node_id_b, 2)
    xor_result = int_a ^ int_b
    return xor_result

node_id_a = '0' * 144 + '00110000'
node_id_b = '0' * 144 + '01000000'

distance = xor_distance(node_id_a, node_id_b)

print(f"XOR Distance (binary): {bin(distance)}")
print(f"XOR Distance (decimal): {distance}")
