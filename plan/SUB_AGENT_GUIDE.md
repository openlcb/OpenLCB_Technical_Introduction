# Sub-Agent Research Guide

## When to Use Sub-Agents

Sub-agents are best used for **research-heavy tasks that require deep investigation** across multiple files, tools, and resources without directly implementing code. They excel at:

- Finding and synthesizing information scattered across large codebases
- Answering complex "how does this work?" questions
- Mapping dependencies and relationships
- Building understanding before implementation

**DO use sub-agents for**:
- Investigating how PlatformIO handles ESP32 build flags and configuration
- Researching JMRI TCP protocol details and configuration specifics
- Deep-diving into OpenMRN source to understand IfTcp implementation
- Exploring Arduino library quirks and GPIO conflict patterns
- Analyzing OpenMRNLite examples to extract implementation patterns

**DO NOT use sub-agents for**:
- Direct code writing (this session's Copilot should write code)
- File editing and content creation (inefficient; wastes tokens)
- Simple file reads or searches (direct tools are faster)
- Verifying that code works (requires live testing)

---

## Proposed Sub-Agent Tasks for Future Sessions

### SA1: PlatformIO + ESP32 Build Configuration Research

**Trigger**: When ready to create platformio.ini template (Task T2.2)

**Research Goal**: Provide Copilot with a complete, tested platformio.ini template for ESP32 + OpenMRNLite with WiFi transport.

**Sub-Agent Prompt**:
```
Research PlatformIO configuration for ESP32 + OpenMRNLite library with TCP/WiFi support.

TASK: Provide a working platformio.ini template with:
1. ESP32 board selection and framework
2. OpenMRNLite library dependency (version/source)
3. Build flags for WiFi support (if needed)
4. Serial monitor configuration (baud rate, port auto-detection)
5. Upload speed and protocol settings
6. Any compiler flags or optimizations for embedded LCC

RETURN: Complete platformio.ini file with inline comments, plus brief notes on:
- Which settings are mandatory vs. optional
- Common customization points (WiFi SSID/password location)
- Known issues or gotchas with ESP32/PlatformIO/OpenMRNLite combo
- Links to official documentation

Search locations: PlatformIO docs, ESP32 Arduino core docs, OpenMRNLite examples, GitHub issues
```

**Expected Output**: Copy-paste-ready platformio.ini + configuration guide  
**Time Estimate**: 30-45 minutes for sub-agent  
**Primary Copilot Task**: Will use output to populate T2.2

---

### SA2: JMRI TCP/GridConnect Protocol Details

**Trigger**: When ready to document JMRI integration (Task T2.4)

**Research Goal**: Understand how JMRI connects to ESP32 via TCP and displays GridConnect messages.

**Sub-Agent Prompt**:
```
Research JMRI TCP connection and GridConnect protocol for LCC/OpenLCB monitoring.

TASK: Document:
1. JMRI LCC Interface Setup
   - How to create a new LCC Interface in JMRI
   - TCP connection settings (default port, hostname, protocol)
   - Configuration steps with exact menu paths (JMRI GUI)

2. GridConnect Protocol
   - Message format: :X[ID][N][Data];
   - How JMRI displays CID/RID/AMD/Init Complete messages
   - Event message format and how to identify events in console

3. Typical Connection Flow
   - How does JMRI discover or connect to an ESP32 node?
   - What does successful connection look like in JMRI console?
   - Example console output for a node startup (CID through Init Complete)

4. Troubleshooting
   - "Connection refused" - what to check
   - "No messages arriving" - common causes
   - How to verify node is actually running

RETURN: Step-by-step guide with:
- Exact JMRI menu navigation
- Screenshots or detailed descriptions
- Expected console output examples
- Common problems and solutions

Search: JMRI documentation, OpenLCB/OpenMRN docs, JMRI GitHub issues
```

**Expected Output**: JMRI setup guide + example console transcripts  
**Time Estimate**: 45-60 minutes for sub-agent  
**Primary Copilot Task**: Will use output to populate T2.4 section

---

### SA3: OpenMRNLite Arduino API Walkthrough [NOW OPTIONAL - Local Examples Available]

**Trigger**: When starting to write async_blink_esp32 sketch (Task T2.1)

**Status**: ⏳ OPTIONAL - OpenMRNLite is now cloned locally!

**Why Optional Now**: With `OpenMRNLite/examples/ESP32/` available in the workspace, you can examine working examples directly instead of having a sub-agent research and document them. This is faster and gives you the actual, tested code patterns.

**When to Use SA3**:
- If local examples are unclear or you need deeper API documentation
- If you want formal documentation of class/function signatures  
- Otherwise: **just examine `OpenMRNLite/examples/ESP32/` directly**

**Sub-Agent Prompt** (if needed):
```
Research OpenMRNLite Arduino library API patterns for event production/consumption.

TASK: Analyze OpenMRNLite examples and documentation to provide:

1. Minimal Arduino Sketch Structure
   - OpenMRNLite initialization sequence
   - SimpleStack setup for TCP/WiFi
   - How to attach GPIO pins and event handlers

2. Event Production Pattern
   - How to produce/send an event in response to button press
   - Code example for detecting GPIO button state change
   - Event ID usage and format

3. Event Consumption Pattern
   - How to consume/receive events
   - Code example for responding to event (e.g., toggle LED)
   - Callback function signature

4. WiFi Connection
   - How does OpenMRNLite handle WiFi on ESP32?
   - Does it use ESP32 built-in WiFi API or wrapper?
   - Configuration approach (hardcoded vs. config file)

5. Node Initialization
   - How is the node ID set?
   - Does alias allocation happen automatically?
   - When are startup messages (CID/RID/AMD/Init Complete) sent?

6. Serial Output & Debugging
   - How to see debug output showing startup sequence?
   - How to monitor event production/consumption?

RETURN: Comprehensive code examples for:
- setUp() function with WiFi/OpenMRNLite init
- Button input handler with event production
- Event consumption callback
- Annotated complete sketch outline

Plus: List of key classes/functions (OpenMRNLite, SimpleStack, etc.)

Search: openmrn/arduino/OpenMRNLite/examples/*, OpenMRNLite source code, Arduino forum posts
```

**Expected Output**: Annotated code examples + API reference  
**Time Estimate**: 60-90 minutes for sub-agent  
**Primary Copilot Task**: Will use output to write async_blink_esp32.ino (T2.1)

---

### SA4: ESP32 GPIO Pin Safety & Conflicts

**Trigger**: When designing breadboard circuit (Task T2.3)

**Research Goal**: Identify safe GPIO pins for button/LED that don't conflict with WiFi or OpenMRNLite.

**Sub-Agent Prompt**:
```
Research ESP32 GPIO pin safety and conflicts for LCC + button + LED application.

TASK: Determine safe GPIO pins for:
1. Button input (produces events)
2. LED output (consumes events)

Analysis required:
1. ESP32 GPIO Pin Categories
   - Which pins are safe to use as general I/O?
   - Which pins are reserved (UART, SPI, boot, strapping)?
   - Which pins are input-only or have special functions?

2. WiFi/OpenMRNLite Conflicts
   - Which pins does ESP32 WiFi module use internally?
   - Which pins might OpenMRNLite library use by default?
   - Any documented GPIO conflicts in OpenMRNLite examples?

3. Recommendation
   - Suggest 2-3 GPIO pins for button input (with rationale)
   - Suggest 2-3 GPIO pins for LED output (with rationale)
   - Avoid: pin 0 (boot button), GPIO 34-39 (input-only), GPIO 6-11 (SPI), etc.

4. Pull-up/Pull-down Considerations
   - Button: needs pull-up resistor? Internal pull-up available?
   - LED: direct GPIO control or current limiting resistor required?

RETURN:
- Pinout table (GPIO, function, safe? Y/N, reason if unsafe)
- Recommended pins for button and LED
- Code snippet for initializing pins (pinMode, digitalWrite, etc.)
- BOM confirmation (resistor values for LED current limiting)

Search: ESP32 datasheet, Arduino ESP32 core documentation, OpenMRNLite examples
```

**Expected Output**: GPIO pin recommendations with safety analysis  
**Time Estimate**: 30-45 minutes for sub-agent  
**Primary Copilot Task**: Will use output to finalize T2.3 (breadboard setup)

---

### SA5: Mermaid Diagram Best Practices for mdBook

**Trigger**: Before creating any diagrams (Tasks T3.1-T3.3)

**Research Goal**: Ensure diagrams render correctly in mdBook and follow best practices.

**Sub-Agent Prompt**:
```
Research Mermaid diagram support in mdBook and best practices.

TASK: Provide guidance for creating Mermaid diagrams in mdBook:

1. mdBook Mermaid Configuration
   - Is Mermaid built-in to mdBook or require plugin?
   - What configuration needed in book.toml?
   - How to enable/disable Mermaid support?

2. Mermaid Syntax for mdBook
   - Correct markdown fence syntax for Mermaid blocks
   - Which diagram types work best: flowchart, sequence, state, etc.?
   - Common syntax gotchas or limitations?

3. Best Practices for Readability
   - Font size and label placement
   - Color usage (light vs. dark themes)
   - Complexity limits before diagram becomes unreadable

4. Testing & Preview
   - How to preview Mermaid diagrams locally (mdbook serve)?
   - Tools for testing Mermaid syntax before committing?
   - Common rendering issues and fixes?

RETURN:
- Complete book.toml Mermaid configuration snippet
- Mermaid markdown fence examples for each diagram type
- Template for sequence diagram (startup sequence)
- Template for state diagram (event state machine)
- Troubleshooting checklist

Search: mdBook documentation, Mermaid documentation, GitHub issues for mdBook+Mermaid
```

**Expected Output**: Mermaid setup + templates ready to use  
**Time Estimate**: 30 minutes for sub-agent  
**Primary Copilot Task**: Will use output to create D1, D2, D3 diagrams

---

## How to Request Sub-Agent Work in Future Sessions

When you want a sub-agent to research something:

1. **Identify the blocker**: What task are you working on? What information is missing?
2. **Define the research goal clearly**: What specific question needs answering?
3. **Use the template above**: Adapt one of the prompts above or create new one following the pattern
4. **Set expectations**: What output format do you expect? (Code examples? Documentation? Analysis?)
5. **Provide context**: Link to relevant files or standards in markdown/
6. **Call the runSubagent tool** with a detailed prompt

**Example Call** (pseudocode):
```
runSubagent(
  description: "Research PlatformIO ESP32 configuration",
  prompt: "[Adapt SA1 prompt from above]"
)
```

---

## Sub-Agent Collaboration Workflow

### Before Starting Implementation

```
Copilot (this session):
  1. Identify research-heavy task (e.g., T2.2 PlatformIO config)
  2. Review RESEARCH.md for what's already known
  3. Craft detailed sub-agent prompt (see templates above)
  4. Launch sub-agent with runSubagent()

Sub-Agent:
  1. Receives prompt and context
  2. Searches workspace, docs, GitHub, standards
  3. Synthesizes findings into concrete output
  4. Returns final report with examples/code/guidance

Copilot (after sub-agent returns):
  1. Review sub-agent output
  2. Update RESEARCH.md with new findings
  3. Use output to implement task (write code, create content)
  4. Update TASKS.md marking task complete
```

### Documenting Sub-Agent Work

After a sub-agent completes research, **update RESEARCH.md** with:
- What was researched and why
- Key findings
- Output produced by sub-agent
- References/links for future sessions

Example:
```markdown
## SA2 Findings: JMRI TCP Configuration (2025-12-19)

Sub-agent researched how to connect ESP32 node to JMRI via TCP.

**Key Findings**:
- JMRI default GridConnect port: 5550
- Connection: Menu → Connections → Add Connection → LCC Interface
- Expected startup message sequence: 4× CID, RID, AMD, Init Complete
- Troubleshooting: Check firewall, verify node is running, check port

**Output**: See [JMRI_Setup_Guide.md](path) in plan/ folder

**References**:
- JMRI Docs: [link]
- OpenLCB Spec: [link]
```

---

## Decision: Sub-Agent vs. Direct Research

**Use Sub-Agent if**:
- Task requires searching across multiple large files
- Deep understanding needed before implementation
- Question is complex or has multiple parts
- Time is limited (delegate research to free up session time)

**Use Direct Tools if**:
- Quick lookup needed (read_file, grep_search)
- Implementing or testing something
- Response needed immediately
- Investigation is targeted/narrow in scope

**Example**:
- ❌ "Research PlatformIO" → Too vague, too broad → Use sub-agent
- ✅ "Check what's in platformio.ini template file X" → Specific → Use read_file directly

---

**Last Updated**: 2025-12-18  
**Ready for**: Next session when implementation tasks arise
