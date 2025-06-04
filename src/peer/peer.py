from typing import Optional, Set
from multiaddr import Multiaddr

from src.id import ID
from src.node.node_id import NodeID
from src.node.node import Node


class Peer:
    """
    BitTorrent Peer implementation.

    A peer is a BitTorrent client/server listening on TCP that implements
    the BitTorrent protocol for uploading/downloading files.
    """

    def __init__(
        self,
        peer_id: ID,
        host: str = "0.0.0.0",
        port: int = 0,
        dht_node: Optional[Node] = None,
    ) -> None:
        self.peer_id = peer_id
        self.host = host
        self.port = port

        # Modern BitTorrent clients also implement DHT nodes
        self.dht_node = dht_node if dht_node is not None else Node()

        # BitTorrent specific attributes
        self.active_torrents: Set[bytes] = set()  # Set of info_hashes
        self.upload_rate: int = 0  # bytes/sec
        self.download_rate: int = 0  # bytes/sec
        self.is_seeder: bool = False

    def get_multiaddr(self) -> Multiaddr:
        """Get the peer's address as a Multiaddr."""
        return Multiaddr(f"/ip4/{self.host}/tcp/{self.port}")

    def add_torrent(self, info_hash: bytes) -> None:
        """Add a torrent to the active torrents list."""
        self.active_torrents.add(info_hash)

    def remove_torrent(self, info_hash: bytes) -> None:
        """Remove a torrent from the active torrents list."""
        self.active_torrents.discard(info_hash)

    def has_torrent(self, info_hash: bytes) -> bool:
        """Check if peer has a specific torrent."""
        return info_hash in self.active_torrents

    def calculate_dht_distance(self, target_node_id: NodeID) -> int:
        """Calculate XOR distance to target DHT node."""
        return self.dht_node.calculate_distance(target_node_id)

    def __repr__(self) -> str:
        return (
            f"<Peer {self.peer_id} {self.host}:{self.port} DHT:{self.dht_node.node_id}>"
        )


class PeerInfo:
    """Extended peer information for BitTorrent context."""

    def __init__(
        self,
        peer_id: ID,
        host: str,
        port: int,
        dht_node_id: Optional[NodeID] = None,
        capabilities: Optional[Set[str]] = None,
    ) -> None:
        self.peer_id = peer_id
        self.host = host
        self.port = port
        self.dht_node_id = dht_node_id
        self.capabilities = capabilities or set()

    def supports_dht(self) -> bool:
        """Check if peer supports DHT."""
        return self.dht_node_id is not None

    def get_multiaddr(self) -> Multiaddr:
        """Get the peer's address as a Multiaddr."""
        return Multiaddr(f"/ip4/{self.host}/tcp/{self.port}")

    def __repr__(self) -> str:
        dht_info = f" DHT:{self.dht_node_id}" if self.dht_node_id else ""
        return f"<PeerInfo {self.peer_id} {self.host}:{self.port}{dht_info}>"
