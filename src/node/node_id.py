import secrets
from typing import Union


class NodeID:
    """160-bit DHT Node ID for Mainline DHT protocol."""

    _bytes: bytes
    _int_value: int | None = None

    def __init__(self, node_id: Union[bytes, int, None] = None) -> None:
        if node_id is None:
            # Generate random 160-bit (20 bytes) node ID
            self._bytes = secrets.token_bytes(20)
        elif isinstance(node_id, bytes):
            if len(node_id) != 20:
                raise ValueError("Node ID must be exactly 20 bytes (160 bits)")
            self._bytes = node_id
        elif isinstance(node_id, int):
            if node_id < 0 or node_id >= (1 << 160):
                raise ValueError("Node ID must be a 160-bit unsigned integer")
            self._bytes = node_id.to_bytes(20, byteorder="big")
        else:
            raise TypeError("Node ID must be bytes, int, or None")

    @property
    def int_value(self) -> int:
        """Get the node ID as an integer for XOR distance calculations."""
        if self._int_value is None:
            self._int_value = int.from_bytes(self._bytes, byteorder="big")
        return self._int_value

    def to_bytes(self) -> bytes:
        """Get the node ID as bytes."""
        return self._bytes

    def to_hex(self) -> str:
        """Get the node ID as a hex string."""
        return self._bytes.hex()

    def xor_distance(self, other: "NodeID") -> int:
        """Calculate XOR distance between this node and another node."""
        return self.int_value ^ other.int_value

    def __eq__(self, other: object) -> bool:
        if isinstance(other, NodeID):
            return self._bytes == other._bytes
        return False

    def __hash__(self) -> int:
        return hash(self._bytes)

    def __repr__(self) -> str:
        return f"<NodeID {self.to_hex()[:16]}...>"

    @classmethod
    def from_hex(cls, hex_str: str) -> "NodeID":
        """Create NodeID from hex string."""
        node_bytes = bytes.fromhex(hex_str)
        return cls(node_bytes)
