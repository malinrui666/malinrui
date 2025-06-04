from .node_id import NodeID
from .node_info import NodeInfo
from .kbucket import RoutingTable


class Node:
    """
    DHT Node implementation for Mainline DHT protocol.

    A node is a DHT client/server listening on UDP that implements
    the Mainline DHT protocol (variant of Kademlia DHT).
    """

    def __init__(
        self, node_id: NodeID | None = None, host: str = "0.0.0.0", port: int = 0
    ) -> None:
        self.node_id: NodeID = node_id if node_id is not None else NodeID()
        self.host: str = host
        self.port: int = port
        # Use K-bucket routing table instead of simple dict
        self.routing_table = RoutingTable(self.node_id, k=8)

    def calculate_distance(self, target_node_id: NodeID) -> int:
        """Calculate XOR distance to target node."""
        return self.node_id.xor_distance(target_node_id)

    def add_node(self, node_info: NodeInfo) -> bool:
        """Add a node to the routing table."""
        return self.routing_table.add_node(node_info)

    def find_closest_nodes(self, target_id: NodeID, k: int = 8) -> list[NodeInfo]:
        """Find K closest nodes to target ID using XOR distance."""
        return self.routing_table.find_closest_nodes(target_id, k)

    def get_routing_table_size(self) -> int:
        """Get the total number of nodes in routing table."""
        return self.routing_table.size()

    def get_bucket_info(self) -> dict[int, int]:
        """Get bucket size information for debugging."""
        return self.routing_table.get_bucket_info()

    def __repr__(self) -> str:
        return f"<Node {self.node_id} {self.host}:{self.port}>"
