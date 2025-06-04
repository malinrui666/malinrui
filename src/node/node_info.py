from .node_id import NodeID


class NodeInfo:
    """Information about a DHT node."""

    def __init__(self, node_id: NodeID, host: str, port: int) -> None:
        self.node_id = node_id
        self.host = host
        self.port = port

    def __repr__(self) -> str:
        return f"<NodeInfo {self.node_id} {self.host}:{self.port}>"
