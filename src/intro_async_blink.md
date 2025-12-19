# Node Startup Sequence

When an OpenLCB node powers up or resets, it goes through a defined startup sequence to join the network. Understanding this sequence is essential to building your own nodes.

## What Happens During Startup

Every node follows this sequence:

1. **Check ID (CID)**: The node sends four CID frames containing its 6-byte unique Node ID, spread across the frames. This allows the node to check if anyone else is using its desired alias.

2. **Reserve ID (RID)**: If no other node objects, the node sends an RID frame to reserve its chosen 12-bit alias for the Node ID.

3. **Alias Map Definition (AMD)**: The node announces the mapping between its full 6-byte Node ID and the 12-bit alias it just reserved.

4. **Initialized (Init Complete)**: The node sends an "Initialized" message telling the network it's now fully online and reachable.

```mermaid
sequenceDiagram
    participant Node as New Node
    participant Bus as OpenLCB Network
    participant Others as Other Nodes
    
    Note over Node: Power On / Reset
    
    Node->>Bus: CID Frame 1 (Node ID bytes 0-1)
    Node->>Bus: CID Frame 2 (Node ID bytes 2-3)
    Node->>Bus: CID Frame 3 (Node ID bytes 4-5)
    Node->>Bus: CID Frame 4 (checks alias)
    
    Note over Node,Others: Wait for conflicts (750ms)
    
    Node->>Bus: RID (Reserve alias)
    Node->>Bus: AMD (Map Node ID to alias)
    Node->>Bus: Initialized Complete
    
    Note over Node: Now reachable on network
    
    Node->>Bus: Producer Identified (events)
    Node->>Bus: Consumer Identified (events)
```

## Why Use Aliases?

OpenLCB uses 6-byte Node IDs to ensure every device in the world has a unique identifier. However, CAN bus headers only have 29 bits available. To fit the sender information plus message type, OpenLCB uses temporary 12-bit aliases that represent the full Node ID during a session.

This alias negotiation happens every time a node starts up. The aliases are not permanent—they're regenerated each time the node powers on.

## Node Participation

Other nodes on the network listen during this startup sequence. If another node is already using the alias the new node wants, it will send a conflict message, forcing the new node to pick a different alias. This ensures all active nodes have unique aliases.

## Multi-Node Network Behavior

The startup sequence isn't just about a single node announcing itself—it's a conversation with the entire network:

**Other nodes participate by:**
- **Listening** to all CID frames to check for alias conflicts
- **Responding** with conflict messages if their alias is being claimed
- **Recording** the Node ID to alias mapping from AMD frames
- **Acknowledging** new nodes with responses to queries

This cooperative behavior ensures:
- No two nodes ever use the same alias simultaneously
- Nodes can discover each other's capabilities
- Gateways and bridges can manage routing efficiently
- Network monitoring tools (like JMRI) can track all active nodes

When your node starts up, it's not alone—the entire network is watching and ready to help it join successfully.

For detailed protocol specifications, see:
- [S-9.7.2.1 CAN Frame Transfer](https://www.nmra.org/sites/default/files/standards/sandrp/OpenLCB/s-9.7.2.1-canframetransfer-2015-02-17.pdf)
- [TN-9.7.2.1 CAN Frame Transfer](https://www.nmra.org/sites/default/files/standards/sandrp/OpenLCB/tn-9.7.2.1-canframetransfer-2016-02-06.pdf)

> **Note**: Future chapters will dive deeper into how the alias generation algorithm works and how to handle collisions in your code.