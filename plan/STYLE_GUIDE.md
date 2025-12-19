# Writing Style Guide for LCC Book

## Overview

This document captures the writing style and tone of the existing book content to ensure consistency across new sections written by Copilot and future contributors.

---

## Tone & Voice

**Overall Tone**: Conversational, approachable, educator-friendly
- Direct address to reader ("you", "we", "let's")
- Assumes technical competence but explains concepts clearly
- Avoids unnecessary jargon; defines technical terms on first use
- Friendly but not casual

**Examples from existing text**:
> "The purpose of this book is to help you get up to speed on OpenLCB so you can start creating LCC products."

> "There are a lot of concepts and we'll go into just enough depth to get you going, without bogging you down with all the details."

> "Let's start with what to some might be confusing..."

---

## Structure & Organization

### Paragraph Style
- **Medium paragraphs**: 3-5 sentences typical
- Avoid single-sentence paragraphs unless emphasis needed
- Use line breaks between paragraphs for readability
- No excessive blank lines

### Headers
- Use descriptive, action-oriented headers
- Format: `## Main Section`, `### Subsection`
- Headers use title case
- Avoid single header at top (use content directly under filename)

### Lists
- Use bullet points for related items without strict order
- Use numbered lists only when steps must be followed in sequence
- Keep list items concise (1-2 sentences max per item)

**Example from existing**:
```
The DevKit has the following features:

* 16 inputs
* 16 outputs
* 4 Tortoises or 8 series LEDs on 12V
* 8 servos
* Expansion ports for daughter boards with additional features like I/O
```

---

## Technical Content

### Concept Explanation
- State purpose/benefit first, details second
- Explain "why" alongside "what"
- Reference standards and technical notes for deeper dives
- Don't repeat information already in intro.md

**Pattern**:
1. Introduce concept/feature
2. Explain why it matters
3. Show how it works
4. Give concrete example
5. Link to standards for details

**Example from existing**:
> "We're going to focus on how to create a board that can be added to an LCC bus. Therefore, we're going to focus on solutions that use the CAN bus."
> 
> *[Why]: CAN bus is the standard transport*
> *[What]: We'll focus on CAN-based solutions*
> *[Trade-off]: "There are other options, such as WiFi, but we won't cover them in this book."*

### Code & Configuration
- Show working examples with context
- Include brief comments in code snippets
- Explain configuration options and why they matter
- Use inline code formatting for file names, parameter names, commands

### Hardware Descriptions
- Include visual descriptions or reference to images when possible
- Explain component choices and trade-offs (see Balazs quote on board selection)
- Provide sourcing information when relevant
- Link to datasheets for reference

**Example style**:
> "The board uses all through-hole components for two reasons. First, because it doesn't require experience soldering surface-mounted parts. And second, perhaps more importantly, the components can be replaced easily if you happen to destroy them as part of testing."

---

## Sentence Structure

### Clarity Preferences
- **Active voice preferred**: "We'll cover X" not "X will be covered"
- **Direct language**: Avoid hedging ("We believe," "It seems") unless truly uncertain
- **Question as rhetorical device**: OK for intro ("How do you get started?") but sparingly
- **Em dash for asides**: Use (—) not (--) for emphasis or clarification

**Examples from text**:
- "Let's start with what to some might be confusing—the names LCC vs OpenLCB."
- "You'll notice that..."
- "We're assuming you've..."

### Word Choice
- **"We"** for collective author voice
- **"You"** to address reader directly
- **"I"** avoided (use "we" instead)
- **"This book"** to reference the current content
- **"The standards"** to point to authoritative references

### Common Phrasings
- "We'll cover..." (forward-looking)
- "There are a lot of..." (acknowledging complexity)
- "So how do you...?" (transitional question)
- "In this book, as in..." (connecting to standards)
- "...for two reasons" (structured explanation)

---

## Formatting Conventions

### Emphasis
- **Bold** for key terms being defined or highlighted
- *Italic* for titles, brand names, board names, or emphasis
- `Code formatting` for technical identifiers, filenames, commands
- Avoid CAPS for emphasis

### References
- Link to sections within book: use markdown links `[Section Name](../file.md#anchor)`
- Link to external standards: reference by name/number (e.g., "S-9.7.3-MessageNetwork")
- Provide context for links ("See the standards for more details")

### Images
- Descriptive alt text: `![Dev Kit photo](images/devkit.jpg)`
- Caption or sentence describing what reader should notice
- Centered or with surrounding context

### Quotes
- Use block quotes for substantial excerpts (blog posts, specifications, expert commentary)
- Indent and separate from body text
- Cite source if possible

**Example from text**:
```
> It took us several months just to select which Nucleos we should be targeting. 
> [... full quote ...]
```

---

## Terminology & Naming

### Consistent Terms
- **OpenLCB** (not "Open LCB", "OpenLCB protocol", etc.) for the standard/protocol
- **LCC** (not "L.C.C." or "l.c.c.") for the NMRA brand
- **Nucleo** (not "NUCLEO") for development boards
- **DevKit** (not "dev kit", "development kit") when referring to Balazs's board
- **CAN bus** (not "CANbus", "CAN-bus", "can bus")
- **WiFi** (not "wifi", "Wi-Fi") 
- **microcontroller** (not "MCU" without first defining, "micro-controller")
- **ESP32** (not "esp32", "ESP-32")

### Acronyms
- Define on first use: "CAN (Controller Area Network)"
- After definition, use acronym without caps in explanations
- Common exceptions (well-known): I/O, LED, ARM, NMRA, PCB

---

## Flow & Transitions

### Between Sections
- Use topic sentences that signal what's coming
- Reference previous concepts when building on them
- Avoid abrupt topic shifts

**Pattern**:
```
[Previous idea established]

Now that we've covered X, let's look at Y...

[New content]
```

### Within Sections
- Start with overview/purpose
- Move from general to specific
- End with forward pointer to next section or "standards for details"

---

## What to Avoid

❌ **Don't**:
- Use colloquialisms ("gonna", "wanna", "thing", "stuff")
- Over-explain (readers have embedded systems experience)
- Repeat content from intro.md or existing standards
- Use abbreviations without definition (except NMRA, I/O, LED)
- Write passive voice when active is clearer
- Use "we feel" or "we believe" unless genuine uncertainty
- Assume familiarity with OpenMRN/OpenMRNLite beyond Arduino level
- Go too deep into C++ implementation details (keep it accessible)

❌ **Tone issues**:
- Too informal/slang
- Too academic/formal
- Condescending
- Over-apologetic ("unfortunately", "sadly")

---

## Example: Good vs. Not-Good

### ❌ NOT-GOOD
> "It is necessary to configure the WiFi parameters before the system can be initialized. The configuration should be done utilizing the configuration management system. It is important that this be done correctly."

### ✅ GOOD (matching existing style)
> "Before you can connect to WiFi, you'll need to configure the network parameters. We'll use the configuration system to set these—make sure you enter them correctly."

---

## For Code Sections

### Sketch Structure
- Include comments explaining what each section does
- Keep inline comments concise ("// Check button state")
- Use meaningful variable names (not `x`, `y`, `val`)
- Show complete, working example (not fragments)

### Configuration Files
- Comment why each setting matters, not just what it does
- Include reasonable defaults with explanation
- Point out which values need user customization

### Comments Pattern
```cpp
// Descriptive comment about what's happening
void setup() {
    // Setup WiFi with configured SSID
    wifi.begin(SSID, PASSWORD);
}
```

Not:
```cpp
// TODO: fix this
// Initialize
void setup() {
    // start wifi
    wifi.begin(ssid, pwd);  // begin wifi connection
}
```

---

## Checklist for New Content

Before submitting new book content, verify:

- [ ] Tone is conversational but competent (assumes embedded C++ experience)
- [ ] Uses "we" and "you" appropriately; no "I"
- [ ] Explains "why" alongside "what"
- [ ] Avoids jargon or defines it on first use
- [ ] Links to standards for detailed technical references
- [ ] No passive voice where active is clearer
- [ ] Consistent terminology (OpenLCB, LCC, WiFi, CAN bus, etc.)
- [ ] Code examples include helpful comments
- [ ] Hardware descriptions explain trade-offs or design choices
- [ ] Transitions between sections are smooth
- [ ] Avoids repeating content from existing sections
- [ ] Uses formatting (bold, italic, code) consistently
- [ ] Paragraph length is reasonable (3-5 sentences typical)
- [ ] No colloquialisms or slang
- [ ] Technical accuracy verified

---

## When in Doubt

- Look at existing intro.md or start.md sections
- Match the structure, tone, and phrasing patterns you see
- Prioritize clarity over conciseness
- Err on side of "more explanation" not "less"
- Link to standards for technical depth beyond scope

---

**Last Updated**: 2025-12-18  
**Based On**: Existing book content (intro.md, start.md)  
**Apply To**: All new book sections written by Copilot or other contributors
