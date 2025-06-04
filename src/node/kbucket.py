import time
from typing import List, Optional, Dict
from dataclasses import dataclass

from .node_id import NodeID
from .node_info import NodeInfo


@dataclass
class NodeEntry:
    """Entry in a K-bucket with additional metadata."""

    node_info: NodeInfo
    last_seen: float
    failed_count: int = 0

    def __post_init__(self):
        if self.last_seen == 0:
            self.last_seen = time.time()


class KBucket:
    """
    A K-bucket that stores at most K nodes within a specific XOR distance range.
    """

    def __init__(self, k: int = 8):
        self.k = k  # Maximum number of nodes in bucket
        self.nodes: List[NodeEntry] = []
        self.last_updated = time.time()

    def is_full(self) -> bool:
        """Check if bucket is at capacity."""
        return len(self.nodes) >= self.k

    def contains(self, node_id: NodeID) -> bool:
        """Check if bucket contains a specific node ID."""
        return any(entry.node_info.node_id == node_id for entry in self.nodes)

    def get_node(self, node_id: NodeID) -> Optional[NodeEntry]:
        """Get node entry by node ID."""
        for entry in self.nodes:
            if entry.node_info.node_id == node_id:
                return entry
        return None

    def add_node(self, node_info: NodeInfo) -> bool:
        """
        Add a node to the bucket.
        Returns True if added successfully, False if bucket is full.
        """
        # Update if node already exists
        existing = self.get_node(node_info.node_id)
        if existing:
            existing.node_info = node_info
            existing.last_seen = time.time()
            existing.failed_count = 0
            # Move to end (most recently seen)
            self.nodes.remove(existing)
            self.nodes.append(existing)
            self.last_updated = time.time()
            return True

        # Add new node if there's space
        if not self.is_full():
            entry = NodeEntry(node_info, time.time())
            self.nodes.append(entry)
            self.last_updated = time.time()
            return True

        return False

    def remove_node(self, node_id: NodeID) -> bool:
        """Remove a node from the bucket."""
        for i, entry in enumerate(self.nodes):
            if entry.node_info.node_id == node_id:
                del self.nodes[i]
                self.last_updated = time.time()
                return True
        return False

    def get_nodes(self) -> List[NodeInfo]:
        """Get all nodes in the bucket."""
        return [entry.node_info for entry in self.nodes]

    def get_oldest_node(self) -> Optional[NodeEntry]:
        """Get the least recently seen node."""
        if not self.nodes:
            return None
        return min(self.nodes, key=lambda x: x.last_seen)

    def __len__(self) -> int:
        return len(self.nodes)


class RoutingTable:
    """
    DHT Routing Table implementation using K-buckets.
    Organizes nodes by XOR distance ranges in 160 buckets.
    """

    def __init__(self, node_id: NodeID, k: int = 8):
        self.node_id: NodeID = node_id
        self.k = k
        # 160 buckets for 160-bit node IDs
        self.buckets: List[KBucket] = [KBucket(k) for _ in range(160)]

    def _get_bucket_index(self, target_id: NodeID) -> int:
        """
        Get bucket index based on XOR distance.
        Bucket i stores nodes with XOR distance in range [2^i, 2^(i+1)).
        """
        distance = self.node_id.xor_distance(target_id)
        if distance == 0:
            return 0  # Should not happen for different nodes

        # Find the position of the highest set bit
        bit_length = distance.bit_length()
        return min(bit_length - 1, 159)

    def add_node(self, node_info: NodeInfo) -> bool:
        """
        Add a node to the appropriate bucket.
        Returns True if added successfully.
        """
        if node_info.node_id == self.node_id:
            return False  # Don't add ourselves

        bucket_index = self._get_bucket_index(node_info.node_id)
        bucket = self.buckets[bucket_index]

        return bucket.add_node(node_info)

    def remove_node(self, node_id: NodeID) -> bool:
        """Remove a node from the routing table."""
        bucket_index = self._get_bucket_index(node_id)
        return self.buckets[bucket_index].remove_node(node_id)

    def find_closest_nodes(self, target_id: NodeID, k: int = 8) -> List[NodeInfo]:
        """
        Find K closest nodes to target ID using XOR distance.
        """
        all_nodes = []

        # Collect nodes from all buckets
        for bucket in self.buckets:
            all_nodes.extend(bucket.get_nodes())

        # Sort by XOR distance to target
        all_nodes.sort(key=lambda node: target_id.xor_distance(node.node_id))

        return all_nodes[:k]

    def get_bucket_nodes(self, bucket_index: int) -> List[NodeInfo]:
        """Get all nodes from a specific bucket."""
        if 0 <= bucket_index < 160:
            return self.buckets[bucket_index].get_nodes()
        return []

    def get_all_nodes(self) -> List[NodeInfo]:
        """Get all nodes from all buckets."""
        all_nodes = []
        for bucket in self.buckets:
            all_nodes.extend(bucket.get_nodes())
        return all_nodes

    def size(self) -> int:
        """Get total number of nodes in routing table."""
        return sum(len(bucket) for bucket in self.buckets)

    def get_bucket_info(self) -> Dict[int, int]:
        """Get information about bucket sizes for debugging."""
        return {
            i: len(bucket) for i, bucket in enumerate(self.buckets) if len(bucket) > 0
        }
